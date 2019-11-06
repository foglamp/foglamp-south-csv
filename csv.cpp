/*
 * FogLAMP south service plugin
 *
 * Copyright (c) 2018 Dianomic Systems
 *
 * Released under the Apache 2.0 Licence
 *
 * Author: Mark Riddoch
 */
#include <csv.h>
#include <config_category.h>
#include <reading.h>
#include <stdio.h>
#include <stdlib.h>
#include <logger.h>
#include <stdexcept>

using namespace std;

/**
 * Constructor for the csv "sensor"
 */
Csv::Csv()
{
}

/**
 * Destructor for the csv "sensor"
 */
Csv::~Csv()
{
}

void Csv::configure(ConfigCategory *config)
{
	setAssetName(config->getValue("asset"));
	setFile(config->getValue("file"));

	m_fp = fopen(m_file.c_str(), "r");
	if (m_fp == NULL)
	{
		throw runtime_error("Unable to open file");
	}
}

/**
 * Take a reading from the csv "sensor"
 */
Reading	Csv::nextValue()
{
double	val1=0.0, val2=0.0;
char	buffer[80], *ptr;
int ch;

	ptr = buffer;
	while ((ch = fgetc(m_fp)) != EOF && ! (ch == '\n')
				 && ptr - buffer < sizeof(buffer))
	{
		*ptr++ = ch;
	}
	*ptr = 0;
	if (ch == EOF)
	{
		fseek(m_fp, 0L, SEEK_SET);
		Logger::getLogger()->info("Reached EOF, resetting file pointer to beginning of csv file");
	}
	
	val1 = strtof(buffer, NULL);
	char* c = strchr(buffer, ',');
	if (c)
	{
		c++;
		val2 = strtof(c, NULL);
	}
	else
	{
		DatapointValue dpv1((double) 0.0);
		Reading reading(m_asset, new Datapoint("ch1", dpv1));
		DatapointValue dpv2((double) 0.0);
		reading.addDatapoint(new Datapoint("ch2", dpv2));
		return reading;
	}
	
	DatapointValue dpv1((double) val1);
	Reading reading(m_asset, new Datapoint("ch1", dpv1));
	DatapointValue dpv2((double) val2);
	reading.addDatapoint(new Datapoint("ch2", dpv2));
	
	// Logger::getLogger()->info("reading=%s", reading.toJSON().c_str());
	
	return reading;
}
