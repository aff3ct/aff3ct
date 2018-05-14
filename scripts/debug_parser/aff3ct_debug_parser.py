#!/usr/bin/python3

import argparse
import struct
import os
import ctypes
import mat4py

class OutputStructure:
    index = 0
    frame_length = 0
    data_format = ""
    hex_format = False
    name = ""
    frames = []

    def __init__(self):
        self.frames = []

    def import_frames(self, lines, key, inter_frame):
        # list line numbers with specified keys
        key_lns = []
        for i in range(0, len(lines)):
            if key in lines[i]:
                key_lns.append(i)

        for key_ln in key_lns:
            first_frame_ln = key_ln + 1 + inter_frame * self.index
            for i in range(0, inter_frame):
                frame = lines[first_frame_ln + i]
                if inter_frame > 1:
                    frame = frame[frame.find("(") + 1: frame.find(")")]
                else:
                    frame = frame[frame.find("[") + 1: frame.find("]")]
                frame = frame.replace(" ", "")
                frame = frame.replace(",", " ")
                frame = frame.split(" ")
                self.frames.append(frame)
        self.hex_format = "0x" in self.frames[0][0]

    def export_as_text(self, path, frame_index=None):
        with open(path, "w") as fout:
            if frame_index is None:
                fout.write(str(len(self.frames)) + "\n")
            else:
                fout.write("1" + "\n")
            fout.write(self.data_format + "\n")
            fout.write(str(self.frame_length) + "\n")

            if frame_index is None:
                for frame in self.frames:
                    self.export_frame_as_text(fout, frame)
            else:
                self.export_frame_as_text(fout, self.frames[frame_index])

    def export_frame_as_text(self, fout, frame):
        for i, value in enumerate(frame):
            if i:
                fout.write(" ")
            fout.write(value)
        fout.write("\n")

    def export_as_source(self, path, frame_index=None):
        export_all = False
        if frame_index is None:
            export_all = True

        # get c_type (signed char, signed short...)
        c_type = self.get_c_type()

        # unable to identify c_type -> exit
        if not c_type:
            return

        with open(path, "w") as fout:
            fout.write("{0} {1}".format(c_type, self.name))
            if export_all:
                fout.write("[{0}]".format(str(len(self.frames))))
            fout.write("[{0}] = ".format(str(self.frame_length)))

            if export_all:
                fout.write("\n{\n")
                for inner_frame_index in range(0, len(self.frames)):
                    if inner_frame_index:
                        fout.write(",\n")
                    self.write_array(fout, inner_frame_index)
                fout.write("\n}")
            else:
                fout.write("\n")
                self.write_array(fout, frame_index)

            fout.write(";\n")

    def write_array(self, fout, frame_index):
        fout.write("{\n\t")
        for i, value in enumerate(self.frames[frame_index]):
            if i:
                fout.write(",")
                if i % 8 == 0:
                    fout.write("\n\t")
                else:
                    fout.write(" ")
            if self.hex_format and "int" in self.data_format:
                if self.data_format == "int8":
                    value = ctypes.c_int8(int(value, 16)).value
                elif self.data_format == "int16":
                    value = ctypes.c_int16(int(value, 16)).value
                elif self.data_format == "int32":
                    value = ctypes.c_int32(int(value, 16)).value
                elif self.data_format == "int64":
                    value = ctypes.c_int64(int(value, 16)).value
                value = str(value)
            fout.write('{:>4}'.format(value))
        fout.write("\n}")

    def get_c_type(self):
        if self.data_format == "int8":
            c_type = "signed char"
        elif self.data_format == "int16":
            c_type = "signed short"
        elif self.data_format == "int32":
            c_type = "signed int"
        elif self.data_format == "int64":
            c_type = "signed long"
        elif self.data_format == "float32":
            c_type = "float"
        elif self.data_format == "float64":
            c_type = "double"
        else:
            c_type = ""
        return c_type

    def export_as_mat(self, path, frame_index=None):
        myframes = []
        sck_name = os.path.basename(path)

        if not self.hex_format:
            if "float" in self.data_format:
                for frame in self.frames:
                    myframe = []
                    for value in frame:
                        myframe.append(float(value))
                    myframes.append(myframe)
            else:
                for frame in self.frames:
                    myframe = []
                    for value in frame:
                        myframe.append(int(value))
                    myframes.append(myframe)
        else:
            if "float" in self.data_format:
                for frame in self.frames:
                    myframe = []
                    for value in frame:
                        myframe.append(float.fromhex(value))
                    myframes.append(myframe)
            else:
                for frame in self.frames:
                    myframe = []
                    for value in frame:
                        value = int(value, 16)
                        if value > 0x7FFFFFFF:
                            value -= 0x100000000
                        myframe.append(value)
                    myframes.append(myframe)

        mat4py.savemat(path + '.mat', dict({sck_name : myframes}));


    def export_as_bin(self, path, frame_index=None):
        if self.data_format == "int8":
            float_code = 0
            sizeof_value = 8
            pack_format = '>b'
        elif self.data_format == "int16":
            float_code = 0
            sizeof_value = 16
            pack_format = '>h'
        elif self.data_format == "int32":
            float_code = 0
            sizeof_value = 32
            pack_format = '>i'
        elif self.data_format == "int64":
            float_code = 0
            sizeof_value = 64
            pack_format = '>q'
        elif self.data_format == "float32":
            float_code = 1
            sizeof_value = 32
            pack_format = '>f'
        elif self.data_format == "float64":
            float_code = 1
            sizeof_value = 64
            pack_format = '>d'
        else:
            "Unknown format"
            return

        with open(path, "wb") as fout:
            if frame_index is None:
                fout.write(struct.pack('>I', len(self.frames)))
            else:
                fout.write(struct.pack('>I', 1))
            fout.write(struct.pack('>I', float_code))
            fout.write(struct.pack('>I', sizeof_value))
            fout.write(struct.pack('>I', self.frame_length))
            if frame_index is None:
                for frame in self.frames:
                    self.export_frame_as_binary(float_code, fout, frame, pack_format)
            else:
                self.export_frame_as_binary(float_code, fout, self.frames[frame_index], pack_format)

    def export_frame_as_binary(self, float_code, fout, frame, pack_format):
        if not self.hex_format:

            if float_code == 1:
                for value in frame:
                    fout.write(struct.pack(pack_format, float(value)))
            else:
                for value in frame:
                    fout.write(struct.pack(pack_format, int(value)))                    
        else:
            if float_code == 1:
                for value in frame:
                    fout.write(struct.pack(pack_format, float.fromhex(value)))
            else:
                for value in frame:
                    if self.data_format == "int8":
                        value = ctypes.c_int8(int(value, 16)).value
                    elif self.data_format == "int16":
                        value = ctypes.c_int16(int(value, 16)).value
                    elif self.data_format == "int32":
                        value = ctypes.c_int32(int(value, 16)).value
                    elif self.data_format == "int64":
                        value = ctypes.c_int64(int(value, 16)).value
                    fout.write(struct.pack(pack_format, value))


def check_positive(value):
    ivalue = int(value)
    if ivalue <= 0:
        raise argparse.ArgumentTypeError("%s is an invalid positive int value" % value)
    return ivalue


def check_directory(value):
    if not os.path.isdir(value):
        raise argparse.ArgumentTypeError("%s is not a directory" % value)
    return value


def adp_parse_args():
    parser = argparse.ArgumentParser()

    parser.add_argument("path", type=str, help="path to the file to be parsed")
    parser.add_argument("--mod", type=str, required=True, help="module to be extracted, ex : Source_random_fast")
    parser.add_argument("--tsk", type=str, required=True, help="task to be extracted, ex : generate")
    parser.add_argument("--txt", help="export as txt", action='store_true')
    parser.add_argument("--bin", help="export as bin", action='store_true')
    parser.add_argument("--src", help="export as c source", action='store_true')
    parser.add_argument("--mat", help="export as matlab .mat file", action='store_true')
    parser.add_argument("--fra", type=check_positive, help="export a single frame, whose index is specified")
    parser.add_argument("-o", "--output", type=check_directory, help="path to the output folder", default="./")

    args = parser.parse_args()

    if not (args.txt or args.bin or args.src or args.mat):
        parser.error('At least one of the export types is required (e.g. --txt)')

    return args


def get_task_ios(lines, key):
    for i in range(0, len(lines)):
        if key in lines[i]:
            line = lines[i]
            line = line.replace("const ", "")
            line = line[line.find("(") + 1: line.find(")")]
            tsk_ios = line.split(', ')
            return tsk_ios
    return []


def get_output_structures(lines, key):
    tsk_ios = get_task_ios(lines, key)
    if not tsk_ios:
        print(key + " not found")
        exit(1)
    inter_frame = 1
    out_structures = []
    for i in range(0, len(tsk_ios)):
        out_structures.append(OutputStructure())
        out_structures[i].index = i
        tsk_arg = tsk_ios[i]
        tsk_arg = tsk_arg.split(" ")
        out_structures[i].data_format = tsk_arg[0]
        tsk_arg = tsk_arg[1].replace("]", "")
        tsk_arg = tsk_arg.split("[")
        out_structures[i].name = tsk_arg[0]
        if "x" in tsk_arg[1]:
            tsk_arg = tsk_arg[1].split("x")
            inter_frame = int(tsk_arg[0])
            out_structures[i].frame_length = int(tsk_arg[1])
        else:
            inter_frame = 1
            out_structures[i].frame_length = int(tsk_arg[1])
    return out_structures, inter_frame


def main():
    args = adp_parse_args()

    # open files and store contents
    with open(args.path, "r") as debug_file:
        lines = debug_file.readlines()

    # build id key mod::tsk
    key = args.mod + "::" + args.tsk

    # fill output structures
    out_structures, inter_frame = get_output_structures(lines, key)

    # get frames
    for out_sct in out_structures:
        out_sct.import_frames(lines, key, inter_frame)

    # some things to check if --fra is selected...
    if args.fra is not None:
        different_n_frames = False
        n_frames = len(out_structures[0].frames)
        for out_sct in out_structures:
            if len(out_sct.frames) != n_frames:
                different_n_frames = True
        if different_n_frames:
            print("Different n_frames detected in the I/Os. Disabling --fra ...")
            args.fra = None
        if args.fra > n_frames:
            print("--fra index larger than actual n_frames. Disabling --fra...")
            args.fra = None

    # export frames as text
    if args.txt:
        for out_sct in out_structures:
            base_path = os.path.join(args.output, out_sct.name)
            out_sct.export_as_text(base_path + ".txt", args.fra)

    # export frames as bin
    if args.bin:
        for out_sct in out_structures:
            base_path = os.path.join(args.output, out_sct.name)
            out_sct.export_as_bin(base_path + ".bin", args.fra)

    # export frames as source
    if args.src:
        for out_sct in out_structures:
            base_path = os.path.join(args.output, out_sct.name)
            out_sct.export_as_source(base_path + ".h", args.fra)

    # export frames as mat file
    if args.mat:
        for out_sct in out_structures:
            base_path = os.path.join(args.output, out_sct.name)
            out_sct.export_as_mat(base_path, args.fra)


if __name__ == "__main__":
    main()
