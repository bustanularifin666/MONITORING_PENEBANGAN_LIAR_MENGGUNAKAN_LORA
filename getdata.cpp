
/********************************************
 *  Gpsd client example
 *  created by s.farmbauer@red-beard-code.de
 ********************************************/

#include <libgpsmm.h>
#include <iostream>
#include <thread>
#include "GpsPosition.h"
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "rs232.h"

#include <string>
using namespace std;

char* no = new char ();
string test;
#define BUF_SIZE 128

//Controlling the thread flow
bool gReadGps = true;

/// Thread which reads all position form the gpsd
/// server
void read_gpsd_data() {
    gpsmm gps_rec("localhost", DEFAULT_GPSD_PORT);

    if (gps_rec.stream(WATCH_ENABLE|WATCH_JSON) == NULL) {
        std::cerr << "No GPSD running.\n";
        return;
    }

    while (gReadGps) {
        struct gps_data_t *newdata;

        if (!gps_rec.waiting(50000000))
            continue;

        if ((newdata = gps_rec.read()) == NULL) {
	 std::cerr << "Read error.\n";
            return;
        } else {
            GPSPosition dummy(newdata);

        }
    }

}
int n;
///Starts the thread which reads the position and stores
///it in a Contaier
int main (int argc, const char* argv[] )
//int main (void)
{  std::string line;
   std::string new1;
    std::thread gps_thread(read_gpsd_data);
    for (int i=0; i<i+1; i++)
    {
        ifstream myfile2 ("/home/pi/sensor.txt");
        while (getline (myfile2,line)) {
        new1=line;
	}
        const  uint8_t *data = reinterpret_cast<const uint8_t*>(new1.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

	GPSPosition last_pos = GPSPosition::get_last_3d_fix_value();
        time_t now_c = chrono::system_clock::to_time_t(last_pos.get_time());
        now_c = now_c + 25200;
        strftime(no, 19, "%d/%m/%y %H:%M:%S  %Z", localtime(&now_c));
        test = no;
        std::cout << "2"
		  ///<< "Time: " << test
                  << " " << last_pos.get_latitude()
                  << " " << last_pos.get_longitude()
                  << " " << last_pos.get_altitude()
                  << " "<<data<<std::endl;
       ofstream myfile("/home/pi/gps.txt");
       if (myfile.is_open()){
        myfile << "2"
	///<< "Time: " << test
        << " " << last_pos.get_latitude()
        << " " << last_pos.get_longitude()
        << " " << last_pos.get_altitude()
        << " "<<data<<std::endl;
        usleep(1000000);
        }
	else printf("unable to open gps.txt or sensor.txt");
//        myfile.close();

   }
    gReadGps = false;
    gps_thread.join();
//    myfile2.close();
    return 0;
}
