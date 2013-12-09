CXXFLAGS =	-O0 -g -Wall -fmessage-length=0  -I. 

LOG_SRC =  log.cpp DataStore.cpp

STALL_SRC = stall.cpp DataStore.cpp

TEST_SRC = test.cpp DataStore.cpp

LIBS = -lmysqlclient

LOG = log.cgi

STLL = stall.cgi

TEST = test.cgi

log: 	$(LOG) 
$(LOG):	$(LOG_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)
	
stall:	$(STALL) 
$(STALL):	$(STALL_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)
	
test:	$(TEST) 
$(TEST):	$(TEST_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:  
	rm -f *.o $(SERVER) 


