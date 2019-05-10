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
double	val;
char	buffer[80], *ptr;
int	ch;

	ptr = buffer;
	while ((ch = fgetc(m_fp)) != EOF && ! (ch == '\n' || ch == ',')
				 && ptr - buffer < sizeof(buffer))
	{
		*ptr++ = ch;
	}
	*ptr = 0;
	if (ch == EOF)
	{
		fseek(m_fp, 0L, SEEK_SET);
	}
	val = strtof(buffer, NULL);
	DatapointValue value(val);
	return Reading(m_asset,new Datapoint(m_asset, value));
}
