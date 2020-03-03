/*

	Ce programme est destiné au projet	

	Copyright 2017 Benjamin Vedder	benjamin@vedder.se

	This file is part of the VESC firmware.

	The VESC firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The VESC firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#include "conf_general.h"
#include <math.h>
#include "mc_interface.h"
#include "timeout.h"
#include "commands.h"
#include "hw.h"

#define GENE_FREQ_RAF_HZ 	1000
#define avg_size  		150


float pwr;
float speed;
float coef;


static volatile bool stop_now = true;
static volatile bool is_running = false;


//Threads
static THD_FUNCTION(gene_thread,arg);
static THD_WORKING_AREA(gene_thread_wa, 1024);

short index = 0;
float TablePwr[avg_size] = {0};
const  float speed_table[7]={10,20,40,55,65,75,90};
const float power_table[7]={0,10,30,60,80,120,200};
const float coef_table[7]={1,1,0.75,0.5,0.25,0.19,0};

void app_custom_start(void)
{
	stop_now = false;
	chThdCreateStatic(gene_thread_wa, sizeof(gene_thread_wa), NORMALPRIO, gene_thread, NULL);
}

void app_custom_stop(void)
{
	stop_now = true;
	while(is_running)	
	{
		chThdSleepMilliseconds(1);
	}
}

void app_custom_configure(app_configuration *conf)
{
	(void) conf;
}

static THD_FUNCTION(gene_thread, arg)
{
	(void)arg;

	is_running = true;

	

	for(;;)
	{
	
	//const OCV_table IQ_OCV_tabular[51];
	//float rap_reduction_gene = 115;
	//float rap_cycle = 0.50;
	float vitesse_pid = 0;
	
	//float vitesse_utilisateur = mc_interface_get_rpm() / rap_reduction_gene;

	//const float f_samp = mc_interface_get_sampling_frequency_now();
	const float input_voltage = GET_INPUT_VOLTAGE();
	const float current_in = mc_interface_get_tot_current_in_filtered();

	const float vitesse_instant = mc_interface_get_rpm();
	const float puissance_moyen = current_in * input_voltage;

	float som_pwr;
	int pwr_index;
	int i=0;
	som_pwr -= TablePwr[index];
	TablePwr[index] = puissance_moyen;
	som_pwr += TablePwr[index];
	index++;
	if(index == avg_size)
	{
		index = 0;
	}

	float puissance_filtre = -som_pwr / avg_size;
							
        if(puissance_filtre<0)
		puissance_filtre=0;

	if(puissance_filtre > power_table[6])
		pwr_index=5;

	for(i=0;i<7;i++)
	{
		if(power_table[i]>puissance_filtre)
		{
		pwr_index=i-1;
		i=7;
		}
	
	}

	if(pwr_index<0)
	pwr_index =0;	

	vitesse_pid =speed_table[pwr_index]+coef_table[pwr_index]*(puissance_filtre-power_table[pwr_index])/2;

	commands_printf("%f", vitesse_pid);
	commands_printf("%f", puissance_filtre);
	commands_printf("%d", pwr_index);


		//mc_interface_set_duty(rap_cycle);
		mc_interface_set_pid_speed(vitesse_pid*172);

	//Sleep pour un temps fonction de la frequence de raffraichissement
		systime_t sleep_time = CH_CFG_ST_FREQUENCY / GENE_FREQ_RAF_HZ;

	//Au moins 1 tick doit etre sleep pour ne pas bloquer les autres threads
		if (sleep_time == 0)
		{
			sleep_time = 1;
		}
		chThdSleep(sleep_time);

		if(stop_now)
		{
			is_running = false;
			return;
		}
	
	//Reset timeout
		timeout_reset();
	}
}


// void get_index(void) /*fonction indépendante de NumberOfPack*/

/*
{
int i=0;

	if (IQ_SOC == 3270000)
	{		
		Index = SAMPLING_SIZE - 2;
		return;
	}

	for(i=0;I<SAMPLING_SIZE;i++)
	{
		if(IQ_OCV_tabular[i].SOC_grade>IQ_SOC)
		{
			Index = i-1;
			return;
		}
	}
}
	

void get_OCV_from_SOC()
{
	get_index();	
	#if HYSTERESYS_CORRECTION==1;

	if(Charging_Status)
	{
		IQ_OCV=

	}


}
*/




