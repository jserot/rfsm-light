task Dimp(
  in bool e;
  in event h;
 out bool s;
  )
{
  int k;
  enum {E0,E1,E2} state = E0;
  while ( 1 ) {
    switch ( state ) {
    case E2:
      s = true;
      wait_ev(h);
      if ( k==0 ) {
        state = E0;
        }
      else if ( k>0 ) {
        k=k-1;
        }
      break;
    case E1:
      s = false;
      wait_ev(h);
      if ( e==false ) {
        k=(2*k)-1;
        state = E2;
        }
      else if ( e==true ) {
        k=k+1;
        }
      break;
    case E0:
      s = false;
      wait_ev(h);
      if ( e==true ) {
        k=1;
        state = E1;
        }
      break;
    }
  }
};
