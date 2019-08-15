[.[]|({
    directory:.directory,
    command:(. as {file:$file, directory:$directory}
        |.command
        |sub("-fno-isolate-erroneous-paths-dereference";"-m32")
        |sub("(?<a>\\s)\\.\\./"; (.a) + "\($directory)/../")
        |sub("/build/\\.\\./"; "/")
        )
    ,
    file:((.directory|sub("build$";""))+(.file|sub("^../";"")))
})]
