odb: odb.cc student-odb.cxx ../../source/aries_odb.cc
	g++ -std=c++17 $^ -o $@ -lodb-mysql -lodb -lodb-boost -lpthread
student-odb.cxx: student.h
	odb -d mysql --std c++11 --generate-query --generate-schema --profile boost/date-time student.h

clean:
	rm -rf odb *-odb.* *.sql