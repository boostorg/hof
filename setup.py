
import os, itertools

prefix = '/// '
include_dir = 'include/'
doc_dir = 'doc/src'
example_dir = 'example'
namespace = 'boost::fit'
declare_template_block = ('struct', 'class', 'template', 'namespace')
declare_template_stmt = ('BOOST_FIT_LIFT_CLASS')

def write_to(dir_name, file, lines):
    content = list((line + "\n" for line in lines))
    if (len(content) > 0):
        print('Writing file:', file)
        open(os.path.join(dir_name, file), 'w').writelines(content)

def drop(iterable, n):
    return itertools.islice(iterable, n, None)

def adjacent(iterable):
    return itertools.izip_longest(iterable, drop(iterable, 1), fillvalue="")

def extract_section(lines):
    taken = itertools.takewhile(lambda x:not (x[1].startswith('--') or x[1].startswith('==')), adjacent(lines))
    return (x for x, y in taken)

def extract_examples(lines):
    result = []
    for i, x in enumerate(lines):
        if x.startswith('Example'): 
            result.append(extract_section(lines[i+1:]))
    return result

def extract_classes(lines):
    result = [[], []]
    which = 1
    for line in lines:
        if line.startswith(declare_template_block) or line.startswith(declare_template_stmt): which = 0
        if line.startswith(('assert')): which = 1
        result[which].append(line)
        if which == 0:
            if (line.startswith('}') or line.startswith(declare_template_stmt)): which = 1
            if (line.startswith(declare_template_block) and line.endswith(';')): which = 1
    return result[0], result[1]

def generate_cpp(lines):
    decls, main = extract_classes((x[4:] for x in lines if x.startswith('    ')))
    yield '#include "example.h"'
    yield 'using namespace {0};'.format(namespace)
    for x in decls: yield x
    yield 'int main() {'
    for x in main: yield '    ' + x
    yield "}"

def write_examples(md, name):
    examples = extract_examples(md)
    for i, example in enumerate(examples):
        write_to(example_dir, '{0}{1}.cpp'.format(name, i), generate_cpp(example))

def insert_header(lines, file):
    for line in lines:
        yield line
        if line.startswith('=='):
            yield ""
            yield "Header"
            yield "------"
            yield ""
            yield "    #include <{0}>".format(file)
            yield ""

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
            name, ext = os.path.splitext(os.path.basename(file))
            write_header(md, name, file)
            write_examples(md, name)

