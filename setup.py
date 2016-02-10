
import os

prefix = '/// '
include_dir = 'include/'
doc_dir = 'doc/src'

def write_to(dir_name, file, lines):
    open(os.path.join(dir_name, file), 'w').writelines((line + "\n" for line in lines))


def extract_section(lines, section):
    pass

def insert_header_each(line, file):
    yield line
    if line.startswith('=='):
        yield ""
        yield "Header"
        yield "------"
        yield ""
        yield "    #include <{0}>".format(file)
        yield ""

def insert_header(lines, file):
    return [line for x in lines for line in insert_header_each(x, file)]

    
def write_header(md, name, file):
    include_file = file[len(include_dir):]
    write_to(doc_dir, name + '.md', insert_header(md, include_file))

def extract_md(file):
    f = open(file)
    return [line[len(prefix):-1] for line in f.readlines() if line.startswith(prefix)]

for root, subdirs, files in os.walk(include_dir):
    print("root", root)
    if 'detail' not in root:
        for f in files: 
            file = os.path.join(root, f)
            md = extract_md(file)
            if len(md) > 0:
                print('Extracting:', file)
                name, ext = os.path.splitext(os.path.basename(file))
                write_header(md, name, file)

