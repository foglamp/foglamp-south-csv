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
	setDatapoint(config->getValue("datapoint"));
	setMultiColumn(config->getValue("multicolumn"));

	m_fp = fopen(m_file.c_str(), "r");
	if (m_fp == NULL)
	{
		throw runtime_error("Unable to open file");
	}

	if (m_multiColumn)
	{
		char buf[1024];

		if (fgets(buf, sizeof(buf), m_fp))
		{
			m_columnCount = 1;
			for (char *p = buf; *p; ++p)
			{
				if (*p == ',')
				{
					m_columnCount++;
				}
			}
		}
		fseek(m_fp, 0L, SEEK_SET);
	}
}

/**
 * Take a reading from the csv "sensor"
 */
Reading	Csv::nextValue()
{
char			buffer[132], *ptr, *eptr;
int			ch;
vector<Datapoint *>	values;

	if (m_multiColumn)
	{
		if (fgets(buffer, sizeof(buffer), m_fp) == NULL)
		{
			fseek(m_fp, 0L, SEEK_SET);
			(void)fgets(buffer, sizeof(buffer), m_fp);
		}

		ptr = buffer;
		int column = 1;
		do {
			double val = strtof(ptr, &eptr);
			DatapointValue dpv((double)val);
			char dpName[80];
			snprintf(dpName, sizeof(dpName), "%s%d", m_datapoint.c_str(), column++);
			values.push_back(new Datapoint(dpName, dpv));
			if (eptr)
			{
				ptr = eptr;
				while (*ptr && (*ptr == ',' || isspace(*ptr)))
				{
					ptr++;
				}
			}
		} while (eptr && *ptr);
	}
	else
	{
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
		double val = strtof(buffer, NULL);
		DatapointValue value(val);
		values.push_back(new Datapoint(m_datapoint, value));
	}

	Reading reading(m_asset, values);
	
	return reading;
}
