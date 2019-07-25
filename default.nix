with import <nixpkgs> {};

let
  py = python36;
  # crc16 does not exist in pythonPackages - bring in our own.
  crc16 = py.pkgs.buildPythonPackage rec {
    pname = "crc16";
    version = "0.1.1";

    src = py.pkgs.fetchPypi {
      inherit pname version;
      sha256 = "15nkx0pa4lskwin84flpk8fsw3jqg6wic6v3s83syjqg76h6my61";
    };
  };
in stdenv.mkDerivation rec {
  name = "card10";
  nativeBuildInputs = [
    bash
    crc16
    gcc-arm-embedded
    meson
    ninja
    py
  ];
  src = ./.;
  buildCommand = ''
    build=$(pwd)/build

    # Copy source over, as dev sources from '.' thend to have odd permissions.
    cp -r $src $(pwd)/src
    cd $(pwd)/src
    # Ensure we have write right to patch shebangs.
    chmod -R +w .

    # The nix sandbox does not have /usr/bin/env bash, patch things up.
    for f in lib/micropython/*.sh; do
      patchShebangs "$f"
    done

    # Actually run the build.
    meson --cross-file card10-cross.ini "$build"
    ninja -C "$build" -j $NIX_BUILD_CORES

    # Copy artifacts to derivation outputs.
    install -D -m 444 "$build/bootloader/bootloader.elf" -t "$out/bootloader"
    install -D -m 444 "$build/pycardium/pycardium_epicardium.bin" -t "$out/pycardium"
    install -D -m 444 "$build/epicardium/epicardium.elf" -t "$out/epicardium"
    install -D -m 444 "$build/pycardium/pycardium.elf" -t "$out/pycardium"
  '';
}
