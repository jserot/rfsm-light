#include "mul8.h"
#include "rfsm.h"


void Mul8::react()
{
  state = Idle;
  while ( 1 ) {
    switch ( state ) {
    case Idle:
      rdy.write(true);
      wait(h.posedge_event());
      if ( start.read()==true ) {
        a=(sc_uint<16> )(n1.read());
        b=n2.read();
        acc=0;
        i=0;
        state = Compute;
        }
      wait(SC_ZERO_TIME);
      break;
    case Compute:
      rdy.write(false);
      wait(h.posedge_event());
      if ( i==8 ) {
        r.write(acc);
        state = Idle;
        }
      else if ( i>0 && b.range(0,0)==0 ) {
        a=a<<1;
        b=b>>1;
        i=i+1;
        }
      else if ( i>0 && b.range(0,0)==1 ) {
        acc=acc+a;
        a=a<<1;
        b=b>>1;
        i=i+1;
        }
      wait(SC_ZERO_TIME);
      break;
    }
  }
};
