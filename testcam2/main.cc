/*
 *  $Id: main.cc,v 1.2 2012-08-13 07:13:12 ueshiba Exp $
 */
#include <cstdlib>
#include "TU/v/vIIDC++.h"
#include "MyCmdWindow.h"

/************************************************************************
*  global functions							*
************************************************************************/
int
main(int argc, char* argv[])
{
    using namespace	TU;
    
    v::App		vapp(argc, argv);
    IIDCCamera::Speed	speed = IIDCCamera::SPD_400M;
    
  // Parse command options.
    extern char*	optarg;
    for (int c; (c = getopt(argc, argv, "b")) != EOF; )
	switch (c)
	{
	  case 'b':
	    speed = IIDCCamera::SPD_800M;
	    break;
	}

  // Main job.
    try
    {
	extern int		optind;
	Array<IIDCCamera>	cameras(argc - optind);
	for (auto& camera : cameras)
	{
	    camera.initialize(strtoull(argv[optind++], 0, 0));
	    camera.setSpeed(speed);
	}

	if (cameras.size() == 0)
	    throw std::runtime_error("One or more cameras must be specified!!");

	v::MyCmdWindow<Array<IIDCCamera>, u_char>	myWin(vapp, cameras);
	vapp.run();

	std::cout << cameras;
    }
    catch (std::exception& err)
    {
	std::cerr << err.what() << std::endl;
	return 1;
    }

    return 0;
}
