target("cpAsyncNetw")
    set_kind("shared")
    set_group("cpAsyncNetw")

    set_basename("cpAsyncNetw")

    -- Files
    add_files("**.cpp")
    add_headerfiles("**.h", "**.hpp")

    --set_pcxxheader("RedSocketPCH.h")
    add_includedirs(".")

    -- Configuration

    -- Dependencies
    add_deps(
        "RED4ext.SDK",
        "RedLib")

    -- Linking
    add_packages("asio")
    add_links("User32")

target_end()