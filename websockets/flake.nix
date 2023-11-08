{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = { self, nixpkgs, flake-utils, ... }:
	flake-utils.lib.eachDefaultSystem (system:
  	let 
      pkgs = import nixpkgs { inherit system; };
      stdenv = pkgs.stdenv;
  	in rec {
      packages.myProject = stdenv.mkDerivation {
        name = "my-project";
        nativeBuildInputs = [
          pkgs.cmake
        ];
        buildInputs = [
          pkgs.boost
          pkgs.gtest
        ];
      };
    	defaultPackage = packages.myProject;
  	});
}