#!/usr/bin/python3

import argparse
import struct
import os


class OutputStructure:
    index = 0
    frame_length = 0
    data_format = ""
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

    def export_as_text(self, path):
        # write output information into file
        with open(path, "w") as fout:
            fout.write(str(len(self.frames)) + "\n")
            fout.write(self.data_format + "\n")
            fout.write(str(self.frame_length) + "\n")

            for frame in self.frames:
                for i, value in enumerate(frame):
                    if i:
                        fout.write(" ")
                    fout.write(value)
                fout.write("\n")

    def export_as_source(self, path, frame_index):
        if frame_index > len(self.frames) - 1:
            print("Specified frame index superior to the number of frames. Frame 0 is exported instead.")
            frame_index = 0

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
            "Unknown format"
            return

        with open(path, "w") as fout:
            fout.write("#ifndef " + self.name.upper() + "_H\n")
            fout.write("#define " + self.name.upper() + "_H\n")
            fout.write("\n")
            fout.write(c_type + " " + self.name + "[" + str(self.frame_length) + "] = {\n\t")
            for i, value in enumerate(self.frames[frame_index]):
                if i:
                    fout.write(",")
                    if i % 8 == 0:
                        fout.write("\n\t")
                    else:
                        fout.write(" ")
                fout.write('{:>4}'.format(value))
            fout.write("\n")
            fout.write("};\n\n")
            fout.write("#endif //" + self.name.upper() + "_H\n")

    def export_as_bin(self, path):
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
            fout.write(struct.pack('>I', len(self.frames)))
            fout.write(struct.pack('>I', float_code))
            fout.write(struct.pack('>I', sizeof_value))
            fout.write(struct.pack('>I', self.frame_length))

            for frame in self.frames:
                for value in frame:
                    if float_code == 1:
                        fout.write(struct.pack(pack_format, float(value)))
                    else:
                        fout.write(struct.pack(pack_format, int(value)))


def adp_parse_args():
    parser = argparse.ArgumentParser()

    parser.add_argument("path", type=str, help="path to the file to be parsed")
    parser.add_argument("--mod", type=str, required=True, help="module to be extracted, ex : Source_random_fast")
    parser.add_argument("--tsk", type=str, required=True, help="task to be extracted, ex : generate")
    parser.add_argument("-o", "--output", type=str, help="path to the output folder", default="./")
    parser.add_argument("--src", type=int, help="export specified frame as .h source file")

    args = parser.parse_args()
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

    # export frames as text

    for out_sct in out_structures:
        base_path = os.path.join(args.output, out_sct.name)
        out_sct.export_as_text(base_path + ".txt")

    # export frames as bin
    for out_sct in out_structures:
        base_path = os.path.join(args.output, out_sct.name)
        out_sct.export_as_bin(base_path + ".bin")

    # export frames as source
    if args.src:
        for out_sct in out_structures:
            base_path = os.path.join(args.output, out_sct.name)
            out_sct.export_as_source(base_path + ".h", args.src)


if __name__ == "__main__":
    main()
