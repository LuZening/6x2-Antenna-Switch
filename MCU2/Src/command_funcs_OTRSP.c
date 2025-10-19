/*
 * command_funcs.c
 *
 *  Created on: Mar 6, 2023
 *      Author: cpholzn
 */

#include <stdint.h>
#include "command_funcs_OTRSP.h"

#include "commands.h"
#include "main.h"
#include "Config/Config.h"





int fn_ping(uint8_t argc, char** argv, char* ret)
{
	ret[0] = '?';
	ret[1] = '\r';
	ret[2] = 0;
	return 0;
}


int fn_get_ant_assignment(uint8_t argc, char** argv, char* ret)
{
	if(argc < 2) goto error;
	uint32_t radionum;
	err_t err;
	err = my_atou(argv[1], 1, &radionum);
	if(err != ERR_OK) goto error;
	if(radionum >= 1 && radionum <= N_SELECTORS)
	{
		uint8_t antnums[N_SELECTORS];
		get_Antenna_real_BCDs(antnums, N_SELECTORS);
		strcpy(ret, "AUX");
		ret[3] = '0' + radionum;
		ret[4] = '0' + antnums[radionum-1];
		ret[5] = '\r';
		ret[6] = 0;
		return 0;
	}

error:
	return 1;
}



int fn_set_ant_assignment(uint8_t argc, char** argv, char* ret)
{
	if(argc < 3) goto error;
	uint32_t radionum;
	uint32_t antnum = 0;
	err_t err;
	err = my_atou(argv[1], 1, &radionum);
	if(err != ERR_OK) goto error;
	err = my_atou(argv[2], 2, &antnum);
	if(radionum >= 1 && radionum <= N_SELECTORS && antnum <= N_ANTENNAS)
	{
		/* send set request */
		uint8_t antnums[N_SELECTORS] = {0};
		for(uint8_t iRadio = 0; iRadio < N_SELECTORS; ++iRadio)
		{
			antnums[iRadio] = Selector[iRadio].sel;
		}
		antnums[radionum - 1] = antnum;
		switch_Antenna(antnums, N_SELECTORS);

		// print antnum to bufCMDRet
		strcpy(ret, "AUX");
		ret[3] = '0' + radionum;
		ret[4] = '0' + antnum;
		ret[5] = '\r';
		ret[6] = 0;
		return 0;
	}

error:
	return 1;
}


int fn_set_ant_name(uint8_t argc, char** argv, char* ret)
{
	if(argc < 3) goto error;
	uint32_t antnum = 0;
	err_t err;
	err = my_atou(argv[1], 1, &antnum);
	if(err != ERR_OK) goto error;

	if(antnum > 0 && antnum <= N_ANTENNAS)
	{
		// capture target ant name
		char* sLabelSaved = cfg.sAntNames[antnum-1];
		strlcpy(sLabelSaved, argv[2], MAX_LEN_ANT_LABEL);
		sLabelSaved[MAX_LEN_ANT_LABEL - 1] = 0; // terminate the string
		// print antnum to bufCMDRet
		strcpy(ret, "NAME");
		ret[4] = '0' + antnum;
		strcpy(ret + 5, sLabelSaved);
		strcat(ret, "\r");
		return 0;
	}

error:
	return 1;
}

int fn_get_ant_name(uint8_t argc, char** argv, char* ret)
{
	if(argc < 2) goto error;
	uint32_t antnum = 0;
	err_t err;
	err = my_atou(argv[1], 1, &antnum);
	if(err != ERR_OK) goto error;

	if(antnum > 0 && antnum <= N_ANTENNAS)
	{
		// capture target ant name
		char* sLabelSaved = cfg.sAntNames[antnum-1];
//		strlcpy(sLabelSaved, argv[2], MAX_LEN_ANT_LABEL);
		sLabelSaved[MAX_LEN_ANT_LABEL - 1] = 0; // terminate the string
		// print antnum to bufCMDRet
		strcpy(ret, "NAME");
		ret[4] = '0' + antnum;
		strcpy(ret + 5, sLabelSaved);
		strcat(ret, "\r");
		return 0;
	}

error:
	return 1;
}


const char* commands_OTRSP[] = {
"?", // ping
"?AUX", // get antenna assignment by radio num
"AUX", // set antenna assignment by radio num
"?NAME", // get antenna name by antenna num
"NAME", // set antenna name by antenna num
};

const cmd_executer_func_t command_executers_OTRSP[] = {
		fn_ping,
		fn_get_ant_assignment,
		fn_set_ant_assignment,
		fn_get_ant_name,
		fn_set_ant_name,
};



int parse_command_OTRSP(const char* s, uint16_t len, char* ret)
{

    uint32_t i  = 0;

    static char buf[16] = {0};
    char* pbuf = buf;

    size_t argc = 0;
    static char* argv[4];

    if(len == 0)
    	len = strlen(s);

    // skip first invalid chars
    while((len > 0)
    		&& !(
    				(*s >= 'A'  && *s <= 'Z')
					|| (*s == '?')
				)
		 )
    {
        ++s;
        --len;
    }
    if(len == 0)
        goto bad_command;


    /* build argv string */
    // 1.read command body
    // detect ? and alphabets
    argv[0] = buf;
    while((len > 0)
    		&& (
    				(*s >= 'A'  && *s <= 'Z')
					|| (*s == '?')
				)
		  )
    {
    	--len;
    	*(pbuf++) = *s;
    	++s;
    }
    if(pbuf == buf)
    	goto bad_command;
    *(pbuf++)= 0; // terminate the command
    ++argc;
    // detect number arguments, each digit takes as 1 argument (if exists)
    while(len > 0 && argc < 4)
    {
    	// skip spaces between command and oprands
		while((len > 0) && (*s == ' ' || *s == ',' || *s == '\t'))
		{
			--len;
			++s;
		}
		if(len > 0 && *s >= '0' && *s <= '9')
		{
			*pbuf = *s;
			argv[argc] = pbuf;

			++argc;
			++s;
			--len;
			*(++pbuf) = 0; // move forward AND terminate the 1st number argument
		}
		else
		{
			goto end_parse;
		}
    }


end_parse:
    i = 0;
    // match a command
    while((i < N_commands_OTRSP) && (strcmp(argv[0], commands_OTRSP[i]) != 0)) i++;
    // command found
    if(i < N_commands_OTRSP)
    {
        cmd_executer_func_t fn = command_executers_OTRSP[i];
        if(fn)
        {
        	int r = fn(argc, argv, ret); // execute command
        	if(r != 0) goto bad_command;
        }
    }
    // bad command
    else
    {
        goto bad_command;
    }
    // return the index of parsed command
    return i;
bad_command:
    return -1;
}
