target("server")
    add_languages("c11")
    add_includedirs("include/server", "include/shared")
    add_files("src/server/*.c")
    add_links("ws2_32")

target("client")
    add_languages("c11")
    add_includedirs("include/client", "include/shared")
    add_files("src/client/*.c")
    add_links("ws2_32")