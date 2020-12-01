let
  # Contains the NixOS package collection. ARTIQ depends on some of them, and
  # you may also want certain packages from there.
  pkgs = import <nixpkgs> {};
  artiq-full = import <artiq-full> { inherit pkgs; };
in
  pkgs.mkShell {
    buildInputs = [
      (pkgs.python3.withPackages(ps: [
        # List desired Python packages here.
        # artiq-full.artiq
        # artiq-full.artiq-comtools
        # The board packages are also "Python" packages. You only need a board
        # package if you intend to reflash that board (those packages contain
        # only board firmware).
        # artiq-full.artiq-board-kc705-nist_clock
        # artiq-full.artiq-board-kasli-wipm
        # from the NixOS package collection:
        # ps.paramiko  # needed for flashing boards remotely (artiq_flash -H)
        # ps.pandas
        # ps.numpy
        # ps.scipy
        # ps.numba
        # (ps.matplotlib.override { enableQt = true; })
        # ps.bokeh
      ]))
      # List desired non-Python packages here
      # artiq-full.openocd  # needed for flashing boards, also provides proxy bitstreams
      # pkgs.spyder
    ];
  }
