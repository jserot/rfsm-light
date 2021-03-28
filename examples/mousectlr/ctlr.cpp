#include "ctlr.h"
#include "rfsm.h"


void Ctlr::react()
{
  state = Idle;
  while ( 1 ) {
    switch ( state ) {
    case Wait:
      wait(clk.posedge_event() | clic.posedge_event());
      if ( clk.read() ) {
        if ( ctr<4 ) {
          ctr=ctr+1;
          }
        else if ( ctr==4 ) {
          notify_ev(simpleClic,"simpleClic");
          state = Idle;
          }
        }
      else if ( clic.read() ) {
        notify_ev(doubleClic,"doubleClic");
        state = Idle;
        }
      wait(SC_ZERO_TIME);
      break;
    case Idle:
      wait(clic.posedge_event());
      ctr=0;
      state = Wait;
      wait(SC_ZERO_TIME);
      break;
    }
  }
};
