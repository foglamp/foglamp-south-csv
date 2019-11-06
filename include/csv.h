#ifndef _CSV_H
#define _CSV_H
/*
 * FogLAMP south service plugin
 *
 * Copyright (c) 2018 Dianomic Systems
 *
 * Released under the Apache 2.0 Licence
 *
 * Author: Mark Riddoch
 */
#include <reading.h>
#include <config_category.h>


class Csv {
	public:
		Csv();
		~Csv();
		Reading		nextValue();
	void	setAssetName(const std::string& assetName)
		{
			m_asset = assetName;
		}
	void	setDatapoint(const std::string& datapoint)
		{
			m_datapoint = datapoint;
		}
	void	setMultiColumn(const std::string& multiColumn)
		{
			if (multiColumn.compare("true") == 0)
				m_multiColumn = true;
			else
				m_multiColumn = false;
		}
	void	setFile(const std::string& file)
		{
			m_file = file;
		}
	void	configure(ConfigCategory *config);

	private:
		std::string	m_asset;
		std::string	m_file;
		std::string	m_datapoint;
		bool		m_multiColumn;
		int		m_columnCount;
		FILE		*m_fp;
};
#endif
