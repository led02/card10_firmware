# This nix expression describes the build and debug environment for the
# firmware. To use it, first install nix and then invoke the "nix-shell"
# command to activate the environment. It will install all dependencies for and
# build and install a patched openocd, before dropping you into a shell where
# everything required for compiling/debugging the firmware is available.

{ pkgs ? import <nixpkgs> {} }:

let
  crc16 = { stdenv, buildPythonPackage, fetchPypi }:
  buildPythonPackage rec {
    pname = "crc16";
    version = "0.1.1";
    src = fetchPypi {
      inherit pname version;
      sha256 = "15nkx0pa4lskwin84flpk8fsw3jqg6wic6v3s83syjqg76h6my61";
      };
  };

  ocd = pkgs.openocd.overrideAttrs(old: {
    name = "openocd-cardio";

    src = pkgs.fetchgit {
      url = "https://git.card10.badge.events.ccc.de/card10/openocd.git";
      rev = "90d828185cea44b29cffb40f2c8aea19282b9130";
      sha256 = "092wg19kjapv9s70b23ckd4j5i8ykk3d7mcl4h8cgl2acwcw8myr";
      fetchSubmodules = true;
    };

    nativeBuildInputs = old.nativeBuildInputs ++ [
      pkgs.which
      pkgs.libtool
      pkgs.autoconf
      pkgs.automake
    ];

    SKIP_SUBMODULE="1";

    preConfigure = ''
      ./bootstrap
    '';
  });

in pkgs.mkShell {

  buildInputs = [
    pkgs.meson
    pkgs.ninja
    pkgs.git
    pkgs.gcc-arm-embedded
    pkgs.jq
    (pkgs.python3.withPackages (ps: [
      ps.pillow
      (ps.callPackage crc16 {})
    ]))
    ocd
    pkgs.gcc-arm-embedded
  ];

}
