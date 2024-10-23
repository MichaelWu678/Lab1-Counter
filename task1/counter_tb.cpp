#include "Vcounter.h" //for the module counter
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env){
    int i;      //counts the number of clock cycles to simulate
    int clk;    //the module clock signal

    Verilated::commandArgs(argc, argv); //init top verilog instance
    Vcounter* top = new Vcounter;       //init trace dump

    //turn on signal tracing, and tell Verilator to dump the waveform data to counter.vcd
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp,99);
    tfp->open("counter.vcd");

    //initialize simulation inputs
    top->clk = 1; //top is the name of the top-level entity, only the top-level signals are visible
    top->rst = 1;
    top->en = 0;

    //run simulation for many clock cycles
    for(i=0;i<300;i++){
        //dump variables into VCD file and toggle clock
        for(clk=0;clk<2;clk++){
            tfp->dump (2*i+clk);    //unit is in ps!!
            top->clk = !top->clk;   //the for loop that toggles the clock
            top->eval ();           //also output the trace for each half of the clock cycle
                                    //and force the model to evaluate on both edges of the clock
        }
        top->rst = (i<2) | (i == 5);//change rst signals during simulation
        top->en = (i>4);            //change en signals during simulation
        if(Verilated::gotFinish())  exit(0);
    }
    tfp->close();
    exit(0);
}
