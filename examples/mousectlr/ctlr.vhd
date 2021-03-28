library ieee;
use ieee.std_logic_1164.all;
use work.rfsm.all;

entity ctlr is
  port(
        clic: in std_logic;
        clk: in std_logic;
        doubleClic: out std_logic;
        simpleClic: out std_logic;
        rst: in std_logic
        );
end entity;

