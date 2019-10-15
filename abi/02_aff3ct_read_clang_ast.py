#!/usr/bin/env python3
import argparse
import fileinput
import json
import os
import re
import subprocess as sp
import sys

# enable additional output and dump internal_db
debug_mode = False

setting_keep_task_method = True

reserved_keywords = [ 'class', 'enum', 'exception', 'for', 'if', 'public', 'protected', 'private', 'struct', 'throw', 'throws', 'union', 'virtual', 'while' ]
# process one method argument
def process_arg(arg_signature, arg_rank):
    arg_is_const = False
    arg_is_scalar = True # false if arg is an array, a ptr or a reference

    # drop default initializer
    arg_str = re.sub(r' *=.*$',r'', arg_signature)
    if arg_str.startswith('const '):
        arg_is_const = True
        arg_str = arg_str[len('const '):]

    # extract arg components:
    # . group 1 = left type specs
    # . group 2 = name
    # . group 3 = right type specs (e.g. array brackets)
    m = re.match(r'(.*)(\b[_a-zA-Z][_0-9a-zA-Z]*\b)(\[.*\])?$', arg_str)
    if m is None:
        sys.stderr.write('parse error: arg name extraction failed')
        sys.exit(1)
    arg_name = m.group(2)
    if m.group(3) is not None:
        arg_type = m.group(1).strip()+' '+m.group(3).strip()
        arg_is_scalar = False
    else:
        arg_type = m.group(1).strip()
        if arg_type.endswith(('&','*')):
            arg_is_scalar = False

    arg_fixme = False
    arg_shape = None
    arg_dir = None

    # detect if arg need additional information (shape?, direction?)
    if not arg_is_scalar:
        # array or ptr: shape undetermined (also assume undertermined for ref)
        arg_shape = '<fixme>'
        arg_fixme = True
        if arg_is_const:
            arg_dir = 'input'
        else:
            # dir undertermined, can be input/output or just output only
            arg_dir = '<fixme>'

    # wrap arg infos in a dict
    arg_entry = dict()
    arg_entry['arg_dir'] = arg_dir
    arg_entry['arg_is_const'] = arg_is_const
    arg_entry['arg_is_scalar'] = arg_is_scalar
    arg_entry['arg_name'] = arg_name
    arg_entry['arg_rank'] = arg_rank
    arg_entry['arg_shape'] = arg_shape
    arg_entry['arg_signature'] = arg_signature
    arg_entry['arg_type'] = arg_type
    arg_entry['arg_fixme'] = arg_fixme

    return arg_entry

# decode method args and add corresponding arg_entries to method_entry
def add_args(method_entry, args):
    arg_i = 0
    for arg in args:
        arg_entry = process_arg(arg, arg_i)
        arg_i = arg_i+1
        method_entry['method_arguments'].append(arg_entry)
        if arg_entry['arg_fixme']:
            method_entry['method_fixme'] = True
    method_entry['method_nb_arguments'] = arg_i

# decode method output and add corresponding out_entry
def add_output(method_entry, out_signature):
    if out_signature == 'void':
        method_entry['method_output'] = None
        return

    out_is_const = False
    out_is_scalar = True
    out_entry = dict()
    out_str = out_signature
    if out_str.startswith('const '):
        out_is_const = True
        out_str = out_str[len('const '):]

    out_type = out_str

    if out_type.endswith(('&','*')):
        out_is_scalar = False

    out_fixme = False
    out_shape = None
    if not out_is_scalar:
        # array or ptr: shape undetermined (also assume undertermined for ref)
        out_shape = '<fixme>'
        out_fixme = True

    out_entry['out_is_const'] = out_is_const
    out_entry['out_is_scalar'] = out_is_scalar
    out_entry['out_shape'] = out_shape
    out_entry['out_signature'] = out_signature
    out_entry['out_type'] = out_type
    out_entry['out_fixme'] = out_fixme

    if out_fixme:
        method_entry['method_fixme'] = True
    method_entry['method_output'] = out_entry

# split the arguments portion of a method into individual args
# - assumes that C++ strings have been cleared
# the principle is to split the string from left to right on each comma at which no open bracket is pending,
# where brackets include '[]' '<>' '{}' '()'
def split_args(arg_str):
    args = []
    if arg_str == '':
        return args

    angle_brackets = 0
    square_brackets = 0
    curly_brackets = 0
    parenthesis = 0
    n = len(arg_str)
    start_i = 0
    for i in range(n):
        c = arg_str[i]
        if c == ',':
            if angle_brackets == 0 and square_brackets == 0 and curly_brackets == 0 and parenthesis == 0:
                if i <= start_i or i == n - 1:
                    sys.stderr.write('parse error: argument list\n')
                    sys.exit(1)
                # found comma with no pending open bracket, extract the corresponding arg chunk
                arg = arg_str[start_i:i].strip()
                args.append(arg)
                start_i = i+1
                continue
        elif c == r'[':
            square_brackets = square_brackets+1
        elif c == r']':
            square_brackets = square_brackets-1

        elif c == r'<':
            angle_brackets = angle_brackets+1
        elif c == r'>':
            angle_brackets = angle_brackets-1

        elif c == r'{':
            curly_brackets = curly_brackets+1
        elif c == r'}':
            curly_brackets = curly_brackets-1

        elif c == r'(':
            parenthesis = parenthesis+1
        elif c == r')':
            parenthesis = parenthesis-1

    # process last arg chunk (or empty string if no args)
    arg = arg_str[start_i:n].strip()
    args.append(arg)

    return args

# process one template arg, mainly to extract its default value if it has one
def process_template_arg(template_arg_signature, template_arg_rank):
    m = re.match(r'([^=]+)(?:=(.+))?', template_arg_signature)
    if m is None:
        sys.stderr.write('parse error: template arg name extraction failed')
        sys.exit(1)
    template_arg_name = m.group(1).strip()
    if m.group(2) is not None:
        template_arg_default = m.group(2).strip()
    else:
        template_arg_default = ''

    template_arg_entry = dict()
    template_arg_entry['template_arg_signature'] = template_arg_signature
    template_arg_entry['template_arg_name'] = template_arg_name
    template_arg_entry['template_arg_default'] = template_arg_default
    return template_arg_entry

# process a template declaration
def process_template(template_signature, template_rank):
    template_entry = dict()
    template_entry['template_signature'] = template_signature
    template_entry['template_args'] = []
    template_args_str = re.sub(r'template *<(.*)>', r'\1', template_signature)
    template_args = split_args(template_args_str)

    template_arg_i = 0
    for template_arg in template_args:
        template_arg_entry = process_template_arg(template_arg, template_arg_i)
        template_arg_i = template_arg_i+1
        template_entry['template_args'].append(template_arg_entry)
    template_entry['template_nb_args'] = template_arg_i

    return template_entry

# process method templates declarations
def add_method_templates(method_entry, templates):
    template_i = 0
    for template in templates:
        template_entry = process_template(template, template_i)
        template_i = template_i+1
        method_entry['method_templates'].append(template_entry)
    method_entry['method_nb_templates'] = template_i

# process class templates declarations
def add_class_templates(class_entry, templates):
    template_i = 0
    for template in templates:
        template_entry = process_template(template, template_i)
        template_i = template_i+1
        class_entry['class_templates'].append(template_entry)
    class_entry['class_nb_templates'] = template_i

# split the template part of a declaration
def split_templates(declaration_str):
    templates = []
    declaration_str = declaration_str.strip()
    if not declaration_str.startswith('template'):
        return (declaration_str, templates)
    angle_brackets = 0
    square_brackets = 0
    curly_brackets = 0
    parenthesis = 0
    n = len(declaration_str)
    start_i = 0
    i = len('template')
    while i < n:
        c = declaration_str[i]
        if c == ' ':
            pass # nothing

        elif c == r'[':
            square_brackets = square_brackets+1
        elif c == r']':
            square_brackets = square_brackets-1

        elif c == r'<':
            angle_brackets = angle_brackets+1
        elif c == r'>':
            angle_brackets = angle_brackets-1

        elif c == r'{':
            curly_brackets = curly_brackets+1
        elif c == r'}':
            curly_brackets = curly_brackets-1

        elif c == r'(':
            parenthesis = parenthesis+1
        elif c == r')':
            parenthesis = parenthesis-1

        elif declaration_str[i:].startswith('template'):
            if angle_brackets == 0 and square_brackets == 0 and curly_brackets == 0 and parenthesis == 0:
                if i <= start_i or i == n-1:
                    sys.stderr.write('parse error: template list\n')
                    sys.exit(1)
                # found template prefix with no pending open bracket, extract the corresponding chunk
                template = declaration_str[start_i:i].strip()
                templates.append(template)

                start_i = i
                i = i+len('template')
        else:
            if angle_brackets == 0 and square_brackets == 0 and curly_brackets == 0 and parenthesis == 0:
                if i <= start_i or i == n-1:
                    sys.stderr.write('parse error: template list\n')
                    sys.exit(1)
                # found something that is not a template, stop here
                template = declaration_str[start_i:i].strip()
                templates.append(template)
                break

        i = i+1

    return (declaration_str[i:].strip(), templates)


# detect a class method and record corresponding entry
# - method_is_inline indicate whether the method is defined in the class (True) or just declared (False)
# - method_suffix store trailing method declaration items (such as "= 0" for pure virtual methods)
def add_method(class_entry, method, method_is_inline, method_suffix, method_templates):
    method_entry = dict()
    class_short_name = class_entry['class_short_name']
    method_is_virtual = False
    method_is_pure_virtual = False
    method_has_throw = False
    method_output = None

    # apply some heuristics to detect method kind
    if method.startswith('static '):
        if method in class_entry['class_static_methods']:
            return class_entry['class_static_methods'][method]
        method_kind = 'static'

    elif re.match(r'(?:explicit )?'+class_short_name+r'(?: *<[^<>]*>)?[^i()]*\(', method) is not None:
        if method in class_entry['class_constructors']:
            return class_entry['class_constructors'][method]
        method_kind = 'constructor'

    elif re.search('[^_a-zA-Z0-9]operator[^ _a-zA-Z0-9]', method) is not None:
        if method in class_entry['class_operators']:
            return class_entry['class_operators'][method]
        method_kind = 'operator'

    elif '~' in method:
        if method in class_entry['class_destructors']:
            return class_entry['class_destructors'][method]
        method_kind = 'destructor'

    else:
        if method in class_entry['class_methods']:
            return class_entry['class_methods'][method]
        method_kind = 'method'

    method_str = method

    # catch template<> (unsupported)
    if method_str.startswith('template<>'):
        return None
    # catch affectations
    if method_kind != 'operator' and re.search(r'= *\(', method_str):
        return None
    # catch return statement
    if re.search(r'\breturn\b', method_str):
        return None

    # detect (pure) virtual methods, remove 'virtual' keyword
    m = re.search(r'\bvirtual\b ', method_str)
    if m is not None:
        method_is_virtual = True
        method_str = re.sub(r'\bvirtual\b ', '', method_str)
        if not method_is_inline and method_suffix.endswith(' = 0'):
            method_is_pure_virtual = True

    # record and remove inline keyword
    m = re.search(r'\binline\b ', method_str)
    if m is not None:
        method_is_inline = True
        method_str = re.sub(r'\binline\b ', '', method_str)

    # record and remove throw keyword
    m = re.search(r'\bthrow\b', method_str)
    if m is not None:
        method_has_throw = True
        method_str = re.sub(r'\bthrow(?:\([^)]*\))?', '', method_str)

    # extract method components (output, name, args) according to kind
    if method_kind == 'constructor':
        method_short_name = class_short_name
        m = re.match(r'(?:explicit )?[_a-zA-Z][_a-zA-Z0-9]*(?: *<[^<>]*>)?[^()]*\((.*)\)$', method_str)
        if m is not None:
            method_args = m.group(1).strip()
        else:
            return None
    elif method_kind == 'destructor':
        method_short_name = '~'+class_short_name
        m = re.match(r'~[_a-zA-Z][_a-zA-Z0-9]*(?: *<[^<>]>)?[^()]*\((.*)\)(?: *noexcept)?(?: *= default)? *$', method_str)
        if m is not None:
            method_args = m.group(1).strip()
        else:
            return None
    elif method_kind == 'operator':
        m = re.match(r'(.*)(operator(?:(?:\(\))|(?:\[\])|(?:->)|(?:[^(]=?))) *\((.*)\)$', method_str)
        method_short_name = None
        if m is not None:
            method_output = m.group(1).strip()
            method_short_name = m.group(2)
            method_args = m.group(3).strip()
        else:
            sys.stderr.write('parse error: operator method name')
            sys.exit(1)
    else:
        if method_kind == 'static':
            method_str = re.sub(r'\bstatic\b ', '', method_str)
        # method_str or static method_str
        m = re.match(r'(.*[^_a-zA-Z])([_a-zA-Z][_a-zA-Z0-9]*)\(((?:.*[_a-zA-Z].*)?)\)$', method_str)
        if m is not None:
            method_output = m.group(1).strip()
            method_short_name = m.group(2)
            method_args = m.group(3).strip()
        else:
            # skip, did not recognize a method
            return None

    if method_short_name in reserved_keywords:
        return None

    # record all method infos in a method_entry
    method_entry['method_signature'] = method
    method_entry['method_kind'] = method_kind
    method_entry['method_is_virtual'] = method_is_virtual
    method_entry['method_is_pure_virtual'] = method_is_pure_virtual
    method_entry['method_is_inline'] = method_is_inline
    method_entry['method_has_throw'] = method_has_throw
    method_entry['method_short_name'] = method_short_name
    method_entry['method_name'] = class_entry['class_name']+'::'+method_short_name
    if method_short_name in class_entry['class_name_accounting']:
        n = 1 + class_entry['class_name_accounting'][method_short_name]
    else:
        n = 1
    method_entry['method_unique_short_name'] = "%s_N%u" % (method_short_name, n)
    method_entry['method_unique_name'] = "%s::%s_N%u" % (class_entry['class_name'], method_short_name, n)
    class_entry['class_name_accounting'][method_short_name] = n

    method_entry['method_fixme'] = False
    # process method args
    method_entry['method_arguments'] = []
    args = split_args(method_args)
    add_args(method_entry, args)

    # process method templates
    method_entry['method_templates'] = []
    add_method_templates(method_entry, method_templates)

    # process output
    if method_kind in [ 'static', 'method' ]:
        # skip operators for now
        add_output(method_entry, method_output)


    # store method in class entry
    if method_kind == 'static':
        class_entry['class_static_methods'][method] = method_entry
    elif method_kind == 'constructor':
        class_entry['class_constructors'][method] = method_entry
    elif method_kind == 'operator':
        class_entry['class_operators'][method] = method_entry
    elif method_kind == 'destructor':
        class_entry['class_destructors'][method] = method_entry
    else:
        class_entry['class_methods'][method] = method_entry

    # store method in all_methods index
    class_entry['class_all_methods_index'][method] = method_entry

    # if one method at least is pure virtual, the class is abstract
    if method_is_pure_virtual:
        class_entry['class_is_abstract'] = True
    # if method needs fixes in args and/or output, record it in 'fixme_methods' as well
    if method_entry['method_fixme']:
        class_entry['class_fixme_methods'][method] = method_entry

    return method_entry

# add a new class to the db
# - name_path_str is the full qualified name including namespace(s)
# - class_inheritence is the right hand side of the class declaration, stating base class(es)
def add_class(db, name_path_str, class_short_name, class_inheritence, class_template_part):
    # initialize a class entry
    class_entry = dict()
    class_entry['class_name'] = name_path_str
    class_entry['class_short_name'] = class_short_name
    if class_inheritence:
        class_entry['class_inheritence'] = class_inheritence
    class_entry['class_constructors'] = dict()
    class_entry['class_destructors'] = dict()
    class_entry['class_static_methods'] = dict()
    class_entry['class_methods'] = dict()
    class_entry['class_operators'] = dict()
    class_entry['class_name_accounting'] = dict()
    class_entry['class_all_methods_index'] = dict()
    class_entry['class_fixme_methods'] = dict()
    class_entry['class_is_abstract'] = False
    class_entry['class_is_module'] = False
    class_entry['class_templates'] = []
    (_,class_templates) = split_templates(class_template_part+' dummy')
    add_class_templates(class_entry, class_templates)
    db[name_path_str] = class_entry
    return class_entry

# initialize a scope entry for the internal database
def new_scope_entry(name_path_str, scope_short_name, scope_kind):
    scope_entry = dict()
    scope_entry['scope_short_name'] = scope_short_name
    scope_entry['scope_name'] = name_path_str
    scope_entry['scope_kind'] = scope_kind
    # attributes belows are index by scope kind keywords, thus no prefix and no plural
    scope_entry['namespace'] = dict()
    scope_entry['class'] = dict()
    scope_entry['method'] = dict()
    return scope_entry

# add a scope_entry (namespace, class or method) to the internal_db
def add_scope(internal_db, prev_scope_entry, name_path_str, scope_short_name, scope_kind):
    if scope_short_name in internal_db:
        scope_entry = internal_db[name_path_str]
    else:
        scope_entry = new_scope_entry(name_path_str, scope_short_name, scope_kind)
        internal_db[name_path_str] = scope_entry
        if prev_scope_entry is not None:
            prev_scope_entry[scope_kind][scope_short_name] = scope_entry

    return scope_entry

# process a Clang -ast-print AST output file
def process_ast(ast_filename):
    print("processing AST file '%s'" % (ast_filename))

    # db storing class/method/arg hierarchy
    db = dict()

    # flat internal db storing all known namespace/class/method scopes
    internal_db = dict()

    # dummy scope for toplevel, outside of any namespace
    scope_entry = add_scope(internal_db, None, '<toplevel>', '<toplevel>', 'toplevel')

    # While True, encountered items should not be recorded in 'db'
    filter_out = True

    # Namespace nesting level at which filter_out was disabled
    # - filter_out should be re-enabled upon exiting this namespace level
    filter_out_threshold = None

    # Scope nesting list
    scopes = []

    # Initial scope is toplevel
    scope = '<toplevel>'
    block_nest_level = 0

    # List of nested namespace/class names
    name_path = [] # name scope path for namespace names and class names

    # Current namespace name and stack of nested namespaces
    namespace_name = ''
    namespace_stack = []

    # Current class and stack of nested classes
    default_class_access = 'private:' # default access in C++ classes is private
    class_stack = []
    class_name = ''
    class_access = default_class_access
    class_entry = None

    # Current method and stack of nested methods
    method = ''
    method_stack = []

    # Main loop processing the pretty-printed AST
    with fileinput.input(ast_filename) as finput:
        for l in finput:
            # workaround on merged lines ending with '= default'
            last_l = False
            while not last_l:
                line_array = l.split(' = default ', 1)
                if len(line_array) > 1:
                    line = line_array[0]+ ' = default '
                    l = line_array[1]
                else:
                    line = line_array[0]
                    last_l = True

                # if debugging, print the line being processed
                if debug_mode:
                    sys.stdout.write('# '+line)

                # basic sanity checks
                line = line.strip()
                if line == '':
                    continue;
                if line == ';':
                    continue;

                # skip using namespace statements for now
                m = re.match(r'^using namespace ', line)
                if m is not None:
                    continue

                # remove literal string contents to avoid mis-counting curly braces
                line = re.sub(r'\\"','', line)
                line = re.sub(r'"[^"]+"','""', line)
                line = re.sub(r"'[^']+'","''", line)

                # count number of opening and closing curlies to detect if a scope boundary is reached
                open_curlies = line.count('{')
                close_curlies = line.count('}')

                # if the number of open curlies does not match the number of closed curlies,
                # this is a scope boundary
                if open_curlies - close_curlies != 0:

                    # if the number of open curlies is larger than the number of close curlies, we are entering some scope(s) (we assume a single scope is entered
                    if open_curlies > close_curlies:
                        # apply some heuristics to detect which kind of scope is entered
                        # - only namespace, class and method scopes are supported for now
                        # - other kinds of scopes (class, union, ...) and basic blocks are ignored

                        # try to match a namespace
                        m = re.match(r'^namespace (.*) {', line)
                        if m is not None:
                            # push previous namespace on the namespace stack
                            namespace_stack.append((namespace_name))

                            # store current scope elements in the scope nesting list
                            scopes.append((scope, scope_entry, block_nest_level))

                            # entering namespace
                            namespace_short_name = m.group(1)
                            name_path.append(namespace_short_name)
                            name_path_str = '::'.join(name_path)
                            namespace_name = name_path_str

                            # create new scope
                            scope = 'namespace'
                            scope_entry = add_scope(internal_db, scope_entry, name_path_str, namespace_short_name, scope)
                            block_nest_level = 1 # 1 accounts for the namespace opening '{'

                            # check if if some wanted namespace is reached
                            # - currently supported:
                            #   - alt3r::module namespace
                            #   - aff3ct::module namespace
                            if filter_out and (name_path_str == 'alt3r::module' or name_path_str == 'aff3ct::module' or name_path_str == 'aff3ct::tools'):
                                # wanted namespace found
                                filter_out_threshold = len(namespace_stack) # record nesting level
                                filter_out = False  # disable filtering until nesting level is left):
                                # wanted namespace found
                                filter_out_threshold = len(namespace_stack) # record nesting level
                                filter_out = False  # disable filtering until nesting level is left

                            if not filter_out:
                                if debug_mode:
                                    print('--> namespace:', namespace_name)
                            continue

                        # ignore 'enum classes' for now
                        if not filter_out and (scope == 'namespace' or scope == '<toplevel>') and not line.startswith('enum class '):
                            # try to match a class
                            m = re.match(r'^(.*)class ([_a-zA-Z][_a-zA-Z0-9]*)(?: : ((?:(?:virtual )?public )?(?:[_a-zA-Z][_a-zA-Z0-9]*::)*[_a-zA-Z][_a-zA-Z0-9]*(?:<[^{]+>)?))? {', line)
                            if m is not None:
                                # push previous class on the class stack
                                class_stack.append((class_name, class_access, class_entry))
                                scopes.append((scope, scope_entry, block_nest_level))

                                # entering class
                                class_template_part = m.group(1).strip()
                                class_short_name = m.group(2)
                                class_inheritence = m.group(3)
                                class_access = default_class_access
                                name_path.append(class_short_name)
                                name_path_str = '::'.join(name_path)
                                class_name = name_path_str

                                # if class not already known, add it to 'db'
                                if name_path_str not in db:
                                    add_class(db, name_path_str, class_short_name, class_inheritence, class_template_part)

                                class_entry = db[name_path_str]
                                scope = 'class'
                                scope_entry = add_scope(internal_db, scope_entry, name_path_str, class_short_name, scope)
                                block_nest_level = 1 # 1 accounts for the class opening '{'

                                if not filter_out:
                                    if debug_mode:
                                        print('==> class_name:', class_name)
                                    if class_template_part != '':
                                        if debug_mode:
                                            print('=== class template part:', class_template_part)
                                continue

                        # try to match a method definition (e.g. with associated block of code)
                        m = re.match(r'^(.*\)) *{', line)
                        # filter out nested functions
                        if m is not None and scope != 'method':
                            method_stack.append(method)
                            scopes.append((scope, scope_entry, block_nest_level))
                            prev_scope_entry = scope_entry
                            method = m.group(1)
                            (method, method_templates) = split_templates(method)
                            if ' : ' in method:
                                # remove any member initializer in constructor
                                method = re.sub(' : .*$', '', method)
                            previous_scope = scope
                            scope = 'method'
                            block_nest_level = 1 # 1 accounts for the namespace opening '{'
                            scope_entry = None
                            if not filter_out:
                                # only record public class members
                                if previous_scope == 'class' and class_access == 'public':
                                    method_entry = add_method(class_entry, method, True, '', method_templates)
                                    if method_entry is not None:
                                        method_unique_name = method_entry['method_unique_name']
                                        method_unique_short_name = method_entry['method_unique_short_name']
                                        scope_entry = add_scope(internal_db, prev_scope_entry, method_unique_name, method_unique_short_name, scope)
                                        if debug_mode:
                                            print('**> method:', method)
                            continue

                    # did not detect any scope of interest, process other cases


                    # update block nesting level in current scope
                    block_nest_level = block_nest_level + open_curlies - close_curlies

                    # sanity check: nesting level should never become negative
                    if block_nest_level < 0:
                        sys.stderr.write('parse_error: block_nest_level = %d\n' % (block_nest_level))
                        sys.exit(1)

                    # nesting level reached 0 for current scope: leaving current scope
                    if block_nest_level == 0:
                        if scope == 'namespace':
                            if not filter_out:
                                if debug_mode:
                                    print('<-- namespace:', namespace_name)
                                    print()

                            # leaving a namespace scope
                            namespace_level = len(namespace_stack)
                            namespace_name = namespace_stack.pop()
                            name_path.pop()
                            name_path_str = '::'.join(name_path)

                            if not filter_out and namespace_level == filter_out_threshold:
                                filter_out = True
                            scope, scope_entry, block_nest_level = scopes.pop()
                            continue

                        elif scope == 'class':
                            if not filter_out:
                                if debug_mode:
                                    print('<== class_name:', class_name)

                            # leaving a class scope
                            class_name, class_access, class_entry = class_stack.pop()
                            name_path.pop()
                            name_path_str = '::'.join(name_path)
                            scope, scope_entry, block_nest_level = scopes.pop()
                            continue

                        elif scope == 'method':
                            if not filter_out:
                                if debug_mode:
                                    print('<** method:', method)

                            # leaving a method scope
                            method = method_stack.pop()
                            scope, scope_entry, block_nest_level = scopes.pop()
                            continue

                        elif scope != '<toplevel>':
                            # leaving any other scope we may have entered
                            # except for toplevel (which should not happen)
                            scope, scope_entry, block_nest_level = scopes.pop()

                # we are not entering or leaving a scope
                elif not filter_out:
                    # detect class access specifiers
                    if scope == 'class' and (line == 'public:' or line == 'protected:' or line == 'private:'):
                        if class_name != '':
                            class_access = line[:-1]
                            if debug_mode:
                                print('=== class access:', class_access)
                            continue

                    # filter out forward declarations
                    m = re.match(r'(?:class|enum|struct|union) (?:[_a-zA-Z][_a-zA-Z0-9]*::)*[_a-zA-Z][_a-zA-Z0-9]*;', line)
                    if m is not None:
                        continue

                    # filter out remaining 'using' statements for now
                    m = re.match(r'using [^;]+;', line)
                    if m is not None:
                        continue

                    # detect method declarations in classes
                    if scope == 'class' and class_access == 'public':
                        method = None

                        (method_line_perhaps, method_templates) = split_templates(line.strip())
                        # heuristic to match a method declaration
                        m = re.match(r'(.*\))([^)]*);', method_line_perhaps)
                        if m is not None:
                            method = m.group(1)
                            method_suffix = m.group(2)
                        else:
                            # special case for '= default' (no trailing ';' in Clang AST print)
                            method_line_perhaps = re.sub(r' throw\(\)', '', method_line_perhaps)
                            m = re.match(r'(.*\))[^)]*( = default)$', method_line_perhaps)
                            if m is not None:
                                method = m.group(1)
                                method_suffix = m.group(2)
                        if method is not None:
                            # some method found, add it to db
                            method_entry = add_method(class_entry, method, False, method_suffix, method_templates)
                            if method_entry is not None:
                                method_unique_name = method_entry['method_unique_name']
                                method_unique_short_name = method_entry['method_unique_short_name']
                                # record a 'pseudo' scope in internal_db
                                add_scope(internal_db, scope_entry, method_unique_name, method_unique_short_name, 'method')
                                if debug_mode:
                                    print ('>>> '+method)
                            continue

                if not filter_out:
                    if scope == 'class' and class_access == 'public':
                        # only consider class members, and only public ones
                        if debug_mode:
                            print(line)
    if debug_mode:
        # if debug_mode, dump internal_db
        internal_db_filename = 'internal_db.json'
        with open(internal_db_filename, 'w') as foutput:
            print("[debug mode] dumping internal_db to '%s'" % (internal_db_filename))
            json.dump(internal_db, foutput)

    return db

# helper function to write a method entry to a file
# - if show_args = True, write method args as well
def write_method(fsummary_output, method_entry, show_args):
    method_kind = method_entry['method_kind']
    method_signature = method_entry['method_signature']
    if method_entry['method_is_pure_virtual']:
        fsummary_output.write(' [%s]  %s\n' % (method_kind[0], method_signature))
    else:
        fsummary_output.write(' -%s-  %s\n' % (method_kind[0], method_signature))
    if show_args:
        if 'method_arguments' in method_entry:
            method_args = method_entry['method_arguments']
            for method_arg in method_args:
                fsummary_output.write('        > %s\n' % (method_arg['arg_signature']))
        if 'method_output' in method_entry:
            method_output = method_entry['method_output']
            if method_output is not None:
                fsummary_output.write('        < %s\n' % (method_output['out_signature']))

# find a method corresponding to a task definition
def find_task_method(class_entry, module_task_entry):
    module_task_name = module_task_entry['task_name']
    module_task_sockets = module_task_entry['sockets']
    module_task_nb_sockets = len(module_task_sockets)
    # lookup all methods
    for method, method_entry in class_entry['class_all_methods_index'].items():
        # early filter out unlikely candidates
        if 'method_output' in method_entry and method_entry['method_output'] is not None:
            continue
        if method_entry['method_short_name'] != module_task_name:
            continue
        method_args = method_entry['method_arguments']
        method_is_task = True
        # also check that the number of arguments and their type match the task sockets
        j = 0
        for i in range(method_entry['method_nb_arguments']):
            method_arg = method_args[i]
            if '=' in method_arg:
                continue
            method_task_socket = module_task_sockets[j]
            method_arg_type = method_arg['arg_type']
            if method_arg_type.endswith(']') and method_task_socket['soc_type']+' []' != method_arg_type:
                method_is_task = False
                break
            elif  method_arg_type.endswith(('*','&')) and method_task_socket['soc_type'] != method_arg_type:
                method_is_task = False
                break
            j = j+1
            if j >= module_task_nb_sockets:
                break
        if method_is_task:
            return method_entry
    return None

# change a class method into a module task
def make_task(class_entry, method_entry, module_task_entry):
    method = method_entry['method_signature']
    # sanity check
    if method_entry['method_kind'] != 'method':
        sys.stderr.write("invalid task kind" + "\n")
        sys.exit(1)

    # change method kind into 'task'
    method_entry['method_kind'] = 'task'
    socket_nb = 0
    for socket in method_entry['method_arguments']:
        if '=' in socket['arg_signature']:
            continue
        # copy() is mandatory here, as keys() aliases the dict keys list which will be modified
        old_keys = socket.copy().keys()
        for old_key in old_keys:
            new_key = 'soc'+old_key[3:]
            if setting_keep_task_method:
                socket[new_key] = socket[old_key]
            else:
                socket[new_key] = socket.pop(old_key)
        socket_rank = socket['soc_rank']
        socket_nb = socket_nb+1
        module_task_socket = module_task_entry['sockets'][socket_rank]
        if socket['soc_shape'] != '<fixme>':
            sys.stderr.write("invalid socket shape" + "\n")
            sys.exit(1)
        socket['soc_shape'] = '<task>' # shape of task sockets is implicitly known
        socket_dir = module_task_socket['soc_dir']

        # normalize keywords
        if socket_dir == 'in':
            socket_dir = 'input'
        elif socket_dir == 'inout':
            socket_dir = 'inputoutput'
        elif socket_dir == 'out':
            socket_dir = 'output'

        if socket['soc_dir'] != '<fixme>' and socket['soc_dir'] != socket_dir:
            sys.stderr.write("invalid socket direction: " + socket['soc_dir'] + "\n")
            sys.exit(1)
        socket['soc_dir'] = socket_dir
        socket['soc_fixme'] = False

    method_entry['method_nb_sockets'] = socket_nb
    if setting_keep_task_method:
        method_entry['method_sockets'] = method_entry['method_arguments']
        class_entry['class_tasks'][method] = method_entry
    else:
        method_entry['method_fixme'] = False
        method_entry.pop('method_nb_arguments')
        method_entry['method_sockets'] = method_entry.pop('method_arguments')
        class_entry['class_tasks'][method] = class_entry['class_methods'].pop(method)
        del class_entry['class_fixme_methods'][method]

# process a modules description .json file
def process_modules(json_modules_filename):
    print("processing a JSON modules description file '%s'" % (json_modules_filename))
    # load the modules db
    with open(json_modules_filename) as json_modules_file:
        modules_db = json.load(json_modules_file)
        if type(modules_db) != dict:
            sys.stderr.write("invalid modules database file format" + "\n")
            sys.exit(1)

    # lookup db classes to check if any is eligible to become a module
    for class_name, class_entry in db.items():
        if class_name in modules_db:
            # the class is a module, add module tasks to the class entry
            module_entry = modules_db[class_name]
            class_entry['class_is_module'] = True
            class_entry['class_tasks'] = dict()
            for module_task_name, module_task_entry in module_entry['tasks'].items():
                # for each module task, find a corresponding method in the class
                task_method_entry = find_task_method(class_entry, module_task_entry)
                if task_method_entry is None:
                    sys.stderr.write('warning: no matching method found for task %s in module %s\n' % (module_task_name, class_name))
                    continue
                # transform the method into a task
                make_task(class_entry, task_method_entry, module_task_entry)

# process a fixes description .json file
def process_fixes(json_fixes_filename):
    # process a 'fixes' .json file
    print("processing a JSON fixes description file '%s'" % (json_fixes_filename))
    # load the fixes db
    with open(json_fixes_filename) as json_fixes_file:
        fix_db = json.load(json_fixes_file)
        if type(fix_db) != dict:
            sys.stderr.write("invalid fixes database file format" + "\n")
            sys.exit(1)

    # try to apply each fix
    for fix_class_name, fix_class_entry in fix_db.items():
        if fix_class_name in db:
            class_entry = db[fix_class_name]
            if len(class_entry['class_fixme_methods']) == 0:
                # class already fixed elsewhere
                continue
            for fix_method_name, fix_method_entry in fix_class_entry.items():
                if fix_method_name in class_entry['class_fixme_methods']:
                    method_entry = class_entry['class_fixme_methods'][fix_method_name]
                    if not method_entry['method_fixme']:
                        continue
                    # prepare for re-checking if all issues are fixed
                    method_fixme = False

                    # check and fix arguments
                    if 'method_arguments' in method_entry:
                        for arg_entry in method_entry['method_arguments']:
                            if not arg_entry['arg_fixme']:
                                continue
                            if arg_entry['arg_name'] in fix_method_entry:
                                fix_arg_entry = fix_method_entry[arg_entry['arg_name']]
                                arg_fixme = False
                                if arg_entry['arg_shape'] == '<fixme>':
                                    if 'shape' in fix_arg_entry:
                                        arg_entry['arg_shape'] = fix_arg_entry['shape']
                                    else:
                                        arg_fixme = True

                                if arg_entry['arg_dir'] == '<fixme>':
                                    if 'dir' in fix_arg_entry:
                                        arg_entry['arg_dir'] = fix_arg_entry['dir']
                                    else:
                                        arg_fixme = True

                                if arg_fixme:
                                    method_fixme = True
                                else:
                                    arg_entry['arg_fixme'] = False

                    # check and fix output
                    if 'method_output' in method_entry:
                        output_entry = method_entry['method_output']
                        if output_entry is not None and output_entry['out_fixme']:
                            if 'RetVal' in fix_method_entry:
                                output_entry['out_shape'] = fix_method_entry['RetVal']
                                output_entry['out_fixme'] = False
                            else:
                                method_fixme = True

                    # check if the fixme status of the method should be updated
                    # - yes: everything is now fixed
                    # - no:  some issues remain
                    if not method_fixme:
                        method_entry['method_fixme'] = False
                        del class_entry['class_fixme_methods'][fix_method_name]

# dump db into a .json file, and dump a side-car text-only summary as well
def dump_db(db, output_filename, summary_output_filename):
    # start by dumping the summary text
    print("dumping db summary to '%s'" % (summary_output_filename))
    with open(summary_output_filename, 'w') as fsummary_output:
        for class_name in sorted(db.keys()):
            class_entry = db[class_name]
            if class_entry['class_is_abstract']:
                class_name = '['+class_name+']'
            if 'inheritence' in class_entry:
                fsummary_output.write('class %s - inheritence: %s\n' % (class_name, class_entry['class_inheritence']))
            else:
                fsummary_output.write('class %s\n' % (class_name))

            # remove name accounting dict to avoid dumping it in the json db
            del class_entry['class_name_accounting']

            for method in class_entry['class_constructors'].keys():
                write_method(fsummary_output, class_entry['class_constructors'][method], True)
            for method in class_entry['class_destructors'].keys():
                write_method(fsummary_output, class_entry['class_destructors'][method], True)
            for method in class_entry['class_methods'].keys():
                write_method(fsummary_output, class_entry['class_methods'][method], True)
            for method in class_entry['class_static_methods'].keys():
                write_method(fsummary_output, class_entry['class_static_methods'][method], True)
            for method in class_entry['class_operators'].keys():
                write_method(fsummary_output, class_entry['class_operators'][method], True)
        # list unresolved issues separately
        first = True
        for class_name in sorted(db.keys()):
            class_entry = db[class_name]
            if class_entry['class_is_abstract']:
                continue
            if len(class_entry['class_fixme_methods']) == 0:
                continue
            if first:
                first = False
                fsummary_output.write('\n')
                fsummary_output.write('=======================\n')
                fsummary_output.write('Warning, fixme methods:\n')
            fsummary_output.write('%s\n' % (class_name))
            for method in class_entry['class_fixme_methods'].keys():
                write_method(fsummary_output, class_entry['class_fixme_methods'][method], False)

    # dump db into a .json file
    print("dumping db to '%s'" % (output_filename))
    with open(output_filename, 'w') as foutput:
        json.dump(db, foutput)

#-------------
# main program

# default Clang AST print file
ast_filename = 'aff3ct_ast.txt'

# build command line arg. parser using Python Standard Lib. 'argparse' module
argparser = argparse.ArgumentParser(description='Build json database from Clang AST, and optional additional sources.')
argparser.add_argument('-m', '--modules', metavar='MODULE_JSON', help='specify a .json file listing module tasks and sockets')
argparser.add_argument('-f', '--fixes', metavar='FIXES_JSON', help='specify a .json file containing manual fixes')
argparser.add_argument('ast_filename', metavar='AST', nargs=argparse.REMAINDER, help='Clang AST obtained from @PROJECT_NAME@_dump_clang_ast.sh')

# parse command line args
args = argparser.parse_args()

# the first positional arg, if any, should be an alternate Clang AST print file
if len(args.ast_filename) > 0:
    ast_filename = args.ast_filename.pop(0)

# process the AST file
db = process_ast(ast_filename)

# process a module file supplied on the command line, if any
if args.modules:
    process_modules(args.modules)

# process a fixes file supplied on the command line, if any
if args.fixes:
    process_fixes(args.fixes)

# dump resulting db
dump_db(db, 'db.json', 'db_summary.txt')

