import os.path as p

DIR_OF_THIS_SCRIPT = p.abspath(p.dirname(__file__))
SOURCE_EXTENSIONS = ['.c', '.s']

database = None

flags = [
    '-Wall',
    '-Wextra',
]

compilation_database_folder = './build'



def IsHeaderFile(filename):
    extension = p.splitext(filename)[1]
    return extension in ['.h']


def FindCorrespondingSourceFile(filename):
    if IsHeaderFile(filename):
        basename = p.splitext(filename)[0]
        for extension in SOURCE_EXTENSIONS:
            replacement_file = basename + extension
            if p.exists(replacement_file):
                return replacement_file
    return filename



def Settings(**kwargs):
    import ycm_core

    global database
    if database is None and p.exists(compilation_database_folder):
        database = ycm_core.CompilationDatabase(compilation_database_folder)

    language = kwargs['language']

    if language == 'cfamily':
        filename = FindCorrespondingSourceFile(kwargs['filename'])

        if not database:
            return {
                'flags': flags,
                'include_paths_relative_to_dir': DIR_OF_THIS_SCRIPT,
                'override_filename': filename
            }

        compilation_info = database.GetCompilationInfoForFile(filename)
        if not compilation_info.compiler_flags_:
            return {}

        final_flags = list(compilation_info.compiler_flags_)
        #raise ValueError(final_flags)
        return {
            'flags': final_flags,
            'include_paths_relative_to_dir':
                compilation_info.compiler_working_dir_,
            'override_filename': filename
        }

    return {}
