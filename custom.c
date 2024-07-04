#include "nwocg_run.h"
#include <math.h>
static struct {
	double input;
	double Gain1;
} nwocg;
void nwocg_generated_init()
{
}
void nwocg_generated_step()
{
	nwocg.Gain1 = nwocg.input * 6.900000;

}
static const nwocg_ExtPort ext_ports[] =
{
	{ "output", &nwocg.Gain1, 0 },
	{ "input", &nwocg.input, 1 },
	{ 0, 0, 0 },
}
const nwocg_ExtPort * const nwocg_generated_ext_ports = ext_ports;
 const size_t nwocg_generated_ext_ports_size = sizeof(ext_ports);
