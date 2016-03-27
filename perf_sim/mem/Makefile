TRUNK=../../

vpath %.h $(TRUNK)/mem/
vpath %.cpp $(TRUNK)/mem/

INCL= -I ./ -I $(TRUNK)/common/

CFLAGS=  -O3 -Ofast -O4 -Wall -std=c++11

miss_rate_sim: cache_tag_array.o miss_rate_sim.o
	$(CXX) -o $@ $^
	@echo "---------------------------------"
	@echo "$@ is built SUCCESSFULLY"

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $^ $(INCL)

-include *.d

clean:
	@-rm *.o *.csv
	@-rm miss_rate_sim