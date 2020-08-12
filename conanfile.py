from conans import ConanFile, Meson, tools


class ConanLiteral(ConanFile):
    name = "strliteral"
    version = "1.0"
    description = "tool used to embed data as string literal"
    license = "MIT"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "meson.build", "strliteral.cpp"

    def build(self):
        meson = Meson(self, build_type="release")
        meson.configure("build")
        meson.build()

    def package(self):
        self.copy("*strliteral", dst="bin", keep_path=False)
