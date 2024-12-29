{
  inputs = {
    utils.url = "github:numtide/flake-utils";
  };
  outputs = {
    self,
    nixpkgs,
    utils,
  }:
    utils.lib.eachDefaultSystem (
      system: let
        pkgs = nixpkgs.legacyPackages.${system};
        inputs = [
          pkgs.gcc
          pkgs.flex
          pkgs.bison
          pkgs.readline
        ];
      in {
        packages = {
          default = self.packages.${system}.cash;
          cash = pkgs.stdenv.mkDerivation {
            pname = "cash";
            version = "1.0";

            src = ./.;

            nativeBuildInputs = inputs;

            installPhase = ''
              mkdir -p $out/bin
              cp cash $out/bin
            '';
          };
        };

        devShell = pkgs.mkShell {
          buildInputs = inputs;
        };
      }
    );
}
