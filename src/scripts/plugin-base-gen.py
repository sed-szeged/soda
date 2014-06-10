#!/usr/bin/python

# Plugin base gen tool generates class files of the specified plugin

from string import Template
import argparse
import os
import re

plugintype = dict()
pluginpath = '../lib/SoDAEngine/inc/engine/plugin'

def main():
    parser = argparse.ArgumentParser(description='plugin-base-gen generates class files of the specified plugin')
    parser.add_argument('-n', '--plugin-name', help='The name of the new plugin.')
    parser.add_argument('-t', '--plugin-type', help='Type of the new plugin')
    parser.add_argument('-l', '--list-types', action='store_true', help='Lists type of the plugins')
    parser.add_argument('-d', '--plugin-directory', default='', help='Path to plugin directory in project directory.')
    parse = parser.parse_args()

    if not os.path.exists(parse.plugin_directory + pluginpath):
        print '[ERROR] Please specifiy plugin-directory path.'
        return
    readPluginTypes(parse.plugin_directory)

    if parse.list_types:
        printPluginTypes()
        return

    if not parse.plugin_name and not parse.plugin_type:
        print '[ERROR] Missing plugin name or plugin type argument.'
        parser.print_help()
        return

    createPluginBase(parse)

def createPluginBase(args):
    pluginname = args.plugin_name
    type = args.plugin_type
    pluginbase = dict()

    pluginbase['className'] = pluginname[0:1].upper() + pluginname[1:] + plugintype[type]['interfacename'][1:]
    pluginbase['directiveName'] = pluginbase['className'].upper() + '_H'
    pluginbase['pluginName'] = pluginname[0:1].lower()
    for i in range(1, len(pluginname)):
        if pluginname[i].isupper():
            pluginbase['pluginName'] += '-'
        pluginbase['pluginName'] += pluginname[i]  

    pluginbase['pluginName'] = pluginbase['pluginName'].lower()
    pluginbase['projectName'] = pluginbase['pluginName'].replace('-', '_') + '_' + type.replace('-', '_')

    pluginpath = args.plugin_directory + '../plugin/' + type.replace('-plugin', '') + '/'

    if os.path.exists(pluginpath + pluginbase['pluginName'] + '/'):
        print '[ERROR] Plugin with the same name already exists'
        return

    with open(pluginpath + 'CMakeLists.txt', 'r+') as f:
        cmakesubdir = 'add_subdirectory(' + pluginbase['pluginName'] + ')\n'
        if cmakesubdir not in f.read():
            f.write(cmakesubdir)

    pluginpath += pluginbase['pluginName'] + '/'
    os.makedirs(pluginpath)

    with open(pluginpath + 'CMakeLists.txt', 'w') as f:
        f.write(createCmakeListsTemplate().substitute(pluginbase))

    pluginpath += pluginname[:1].upper() + pluginname[1:] + plugintype[type]['interfacename'][1:]
    with open(pluginpath + '.cpp', 'w') as f:
        f.write(Template(plugintype[type]['cppformat']).substitute(pluginbase))
    with open(pluginpath + '.h', 'w') as f:
        f.write(Template(plugintype[type]['headerformat']).substitute(pluginbase))

    print '[INFO] Done.'

# prints the available plugin types
def printPluginTypes():
    print 'Available plugin types:'
    for key in plugintype:
        print ' * ' + key

# creates cmakelists template string
def createCmakeListsTemplate():
    cmaketemplate = 'project(${projectName})\n\n'
    cmaketemplate += 'include_directories($${${projectName}_SOURCE_DIR}/../../../lib/SoDA/inc\n'
    cmaketemplate += '                    $${${projectName}_SOURCE_DIR}/../../../lib/SoDAEngine/inc\n'
    cmaketemplate += '                    $${Boost_INCLUDE_DIRS})\n\n'
    cmaketemplate += 'file(GLOB_RECURSE headers ./*.h)\n'
    cmaketemplate += 'aux_source_directory($${${projectName}_SOURCE_DIR} ${projectName}_src)\n\n'
    cmaketemplate += 'add_library(${projectName} SHARED $${headers} $${${projectName}_src})\n'
    cmaketemplate += 'target_link_libraries(${projectName} SoDAEngine SoDA $${Boost_LIBRARIES})'
    return Template(cmaketemplate)

# generates template string based on the actual plugin interface files
def readPluginTypes(plugindir):
    pluginterface = plugindir + pluginpath
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
                    if 'getName()' in line:
                        line += '\n{\n    return "${pluginName}";\n}\n\n'
                    elif 'getDescription()' in line:
                        line += '\n{\n    return "";\n}\n\n'
                    else:
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
