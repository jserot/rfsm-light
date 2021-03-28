library ieee;
use ieee.std_logic_1164.all;
use work.rfsm.all;

entity mul8 is
  port(
        h: in std_logic;
        n1: in integer range 0 to 255;
        n2: in integer range 0 to 255;
        start: in std_logic;
        r: out integer range 0 to 65535;
        rdy: out std_logic;
        rst: in std_logic
        );
end entity;

architecture RTL of mul8 is
  type t_state is ( Compute, Idle );
  signal state: t_state;
begin
  process(rst, h)
    variable a: integer range 0 to 65535;
    variable acc: integer range 0 to 65535;
    variable b: integer range 0 to 255;
    variable i: integer range 0 to 8;
  begin
    if ( rst='1' ) then
      state <= Idle;
    elsif rising_edge(h) then 
      case state is
      when Idle =>
        if ( start = '1' ) then
          a := n1;
          b := n2;
          acc := 0;
          i := 0;
          state <= Compute;
        end if;
      when Compute =>
        if ( i<8 and b(0 downto 0) = 0 ) then
          a := shift_left(a,1);
          b := shift_right(b,1);
          i := i+1;
        elsif  ( i<8 and b(0 downto 0) = 1 ) then
          acc := acc+a;
          a := shift_left(a,1);
          b := shift_right(b,1);
          i := i+1;
        elsif  ( i = 8 ) then
          r <= acc;
          state <= Idle;
        end if;
    end case;
    end if;
  end process;
  process(state)
  begin
    case state is
    when Compute =>
      rdy <= '0';
    when Idle =>
      rdy <= '1';
    end case;
  end process;
end architecture;
