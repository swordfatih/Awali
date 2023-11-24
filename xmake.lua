target("server")
    add_languages("c11")
    add_includedirs("include/server")
    add_files("src/server/*.c")
    add_links("ws2_32")

target("client")
    add_languages("c11")
    add_includedirs("include/client")
    add_files("src/client/*.c")
    add_links("ws2_32")

target("game")
    add_languages("c11")
    add_includedirs("include/game")
    add_files("src/game/*.c")
    add_links("ws2_32")