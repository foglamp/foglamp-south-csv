#ifndef _CSV_H
#define _CSV_H
/*
 * Fledge south service plugin
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
	void	setFile(const std::string& file)
		{
			m_file = file;
		}
	void	configure(ConfigCategory *config);

	private:
		std::string	m_asset;
		std::string	m_file;
		FILE		*m_fp;
};
#endif
