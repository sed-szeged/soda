#!/usr/bin/python

# Plugin base gen tool generates class files for the specified new plugin

from string import Template
import argparse
import os
import re

plugintype = dict()

def main():
    readPluginTypes()
    parser = argparse.ArgumentParser(description='test-suite-score-json-gen generates json file for test-suite-score')                          
    parser.add_argument('-n', '--plugin-name', help='The name of the new plugin.')                                        
    parser.add_argument('-t', '--plugin-type', help='Type of the new plugin')
    parser.add_argument('-l', '--list-types', action='store_true', help='Lists type of the plugins')
    parse = parser.parse_args()
    if parse.list_types:
        printPluginTypes()
        return

    if not parse.plugin_name and not parse.plugin_type:
        print '[ERROR] Missing plugin name or plugin type argument.'
        parser.print_help()
        return

    createPluginBase(parse)
    print 'Done.'

def createPluginBase(args):
    pluginname = args.plugin_name
    type = args.plugin_type
    pluginbase = dict()
    pluginbase['className'] = pluginname[0:1].upper() + pluginname[1:] + plugintype[type]['interfacename'][1:]
    pluginbase['directiveName'] = pluginbase['className'].upper() + '_H'
    pluginbase['projectname'] = pluginname.lower() + '_' + type.replace('-', '_')
#    print Template(plugintype[type]['headerformat']).substitute(pluginbase)
#    print Template(plugintype[type]['cppformat']).substitute(pluginbase)
    pluginpath = './' + type.replace('-plugin', '') + '/'
    with open(pluginpath + 'CMakeLists.txt', 'r+') as f:
        cmakesubdir = 'add_subdirectory(' + pluginname.lower() + ')\n'
        if cmakesubdir not in f.read():
            f.write(cmakesubdir)

    pluginpath += pluginname.lower() + '/'
    if not os.path.exists(pluginpath):
        os.makedirs(pluginpath)

    with open(pluginpath + 'CMakeLists.txt', 'w') as f:
        f.write(createCmakeListsTemplate().substitute(pluginbase))

    pluginpath += pluginname[:1].upper() + pluginname[1:] + plugintype[type]['interfacename'][1:]
    with open(pluginpath + '.cpp', 'w') as f:
        f.write(Template(plugintype[type]['cppformat']).substitute(pluginbase))
    with open(pluginpath + '.h', 'w') as f:
        f.write(Template(plugintype[type]['headerformat']).substitute(pluginbase))

# prints the available plugin types
def printPluginTypes():
    print 'Available plugin types:'
    for key in plugintype:
        print ' * ' + key

# creates cmakelists template string
def createCmakeListsTemplate():
    cmaketemplate = 'project(${projectname})\n\n'
    cmaketemplate += 'include_directories($${${projectname}_SOURCE_DIR}/../../../lib/SoDA/inc\n'
    cmaketemplate += '                    $${${projectname}_SOURCE_DIR}/../../../lib/SoDAEngine/inc\n'
    cmaketemplate += '                    $${Boost_INCLUDE_DIRS})\n\n'
    cmaketemplate += 'file(GLOB_RECURSE headers ./*.h)\n'
    cmaketemplate += 'aux_source_directory($${${projectname}_SOURCE_DIR} ${projectname}_src)\n\n'
    cmaketemplate += 'add_library(${projectname} SHARED $${headers} $${${projectname}_src})\n'
    cmaketemplate += 'target_link_libraries(${projectname} SoDAEngine SoDA $${Boost_LIBRARIES})'
    return Template(cmaketemplate)

# generates template string based on the actual plugin interface files
def readPluginTypes():
    pluginterface = '../lib/SoDAEngine/inc/engine/plugin'
    for filename in os.listdir (pluginterface):
        # process filename for easier input parameters
        type = filename[1]
        for i in range(2, len(filename)-2):
            if filename[i].isupper():
                type += '-'
            type += filename[i]
        type = type.lower()
        plugintype[type] = dict()
        plugintype[type]['filename'] = filename
        # create formatstring
        headerformat = ''
        cppformat = '#include "${className}.h"\n\nnamespace soda {\n\n'
        skip = 0
        interfacename = filename[:len(filename)-2]
        typedefs = list()
        with open(pluginterface + '/' + filename) as f:
            for line in f:
                tmp = line.strip()
                if skip == 0 and (tmp == '/*' or tmp == '/**'):
                    skip = 1
                if skip == 1:
                    if tmp == '*/':
                        skip = 0
                    continue
                if '#' in tmp or 'using namespace' in tmp:
                    continue
                if 'typedef' in tmp:
                    typedefs.append(tmp.split(' ')[tmp.count(' ')].replace(';', ''))
                    continue

                if interfacename in line:
                    if 'class' not in line:
                        line = line.replace(interfacename, '${className}')
                    else:
                        line = line.replace(interfacename, '${className} : public ' + interfacename)
                if '~' in line:
                    headerformat += '    ${className}();\n'
                    cppformat += '${className}::${className}()\n{\n}\n\n'

                line = line.replace('virtual ', '')
                line = line.replace(' {}', ';')
                line = line.replace(' = 0', '')
                headerformat += line

                if 'virtual' in tmp:
                    line = line.strip()
                    line = line.replace(';', '')
                    if '~' in line:
                        line = line.replace('~', '${className}::~')
                    else:
                        methodname = line.split(' ')
                        for v in typedefs:
                            if v in methodname[0]:
                                line = line.replace(methodname[0], '${className}::' + methodname[0])
                        line = line.replace(methodname[1], '${className}::' + methodname[1])
                    line += '\n{\n}\n\n'
                    cppformat += line

                headerformat = headerformat.lstrip('\n')
                headerformat = headerformat.replace('public:\n\npublic:\n', 'public:\n')

            headerformat = '#ifndef ${directiveName}\n#define ${directiveName}\n\n#include "engine/CKernel.h"\n\n' + headerformat[:]
            headerformat += '#endif /* ${directiveName} */\n'
            cppformat += 'extern "C" void registerPlugin(CKernel &kernel)\n'
            cppformat += '{\n    kernel.get' + interfacename[1:] + 'Manager().addPlugin(new ${className}());\n}\n\n'
            cppformat += '} /* namespace soda */\n'

        plugintype[type]['headerformat'] = headerformat
        plugintype[type]['cppformat'] = cppformat
        plugintype[type]['interfacename'] = interfacename


if __name__ == '__main__':
    main()
