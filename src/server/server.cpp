#include "server_session.h"

using namespace server_session;

int main() {
  session session(12345); 
  session.Start();
}
