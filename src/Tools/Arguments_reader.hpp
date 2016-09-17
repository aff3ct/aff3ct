/*!
 * \file    Arguments_reader.hpp
 * \brief   Command line arguments management class.
 * \author  A. Cassagne
 * \date    2014
 *
 * \section LICENSE
 * This file is under CC BY-NC-ND license (http://creativecommons.org/licenses/by-nc-nd/4.0/legalcode).
 *
 * \section DESCRIPTION
 * This is the traditional entry file for the code execution.
 */
#ifndef ARGUMENTS_READER_HPP_
#define ARGUMENTS_READER_HPP_

#include <string>
#include <vector>
#include <map>

/*!
 * \class  Arguments_reader
 * \brief  This class allow us to easily manage arguments from the command line.
 */
class Arguments_reader
{
private:
	std::vector<std::string>                                     m_argv;          /*!< Simple copie des données de "char** argv". */
	std::map<std::vector<std::string>, std::vector<std::string>> m_required_args; /*!< La liste des arguments obligatoires. */
	std::map<std::vector<std::string>, std::vector<std::string>> m_optional_args; /*!< La liste des arguments facultatifs. */
	std::map<std::vector<std::string>, std::string>              m_args;          /*!< La liste des arguments et des valeurs de ces derniers (après parsing). */
	std::string                                                  m_program_name;  /*!< Le nom de l'executable du programme. */

	unsigned int max_n_char_arg;

public:
	/*!
	 *  \brief Constructeur.
	 *
	 *  Le contructeur prend les fameux "int argc" et "char** argv" de la fonction main :-).
	 *
	 *  \param argc : Le nombre d'arguments.
	 *  \param argv : Le tableau des arguments
	 */
	Arguments_reader(const int argc, const char** argv);

	/*!
	 *  \brief Destructeur.
	 *
	 *  Le destructeur ne fait rien...
	 */
	virtual ~Arguments_reader();

	/*!
	 *  \brief Parse "m_argv".
	 *
	 *  Parse "m_argv" selon une liste des arguments requis et facultatifs.
	 *
	 *  \param required_args : Dictionnaire des arguments attendus obligatoires.
	 *  \param optional_args : Dictionnaire des arguments attendus facultatifs.
	 *
	 *  \return Vrai si tous les arguments requis sont bien présents.
	 */
	bool parse_arguments(const std::map<std::vector<std::string>, std::vector<std::string>> &required_args,
	                     const std::map<std::vector<std::string>, std::vector<std::string>> &optional_args);

	/*!
	 *  \brief Cherche si un agument existe.
	 *
	 *  \param tags : Tags de l'argument recherché.
	 *
	 *  \return Vrai si l'argument existe (à utiliser après parse_arguments).
	 */
	bool exist_argument(const std::vector<std::string> &tags);
	bool exist_arg     (const std::vector<std::string> &tags) { return exist_argument(tags); }

	/*!
	 *  \brief Retourne la valeur d'un argument.
	 *
	 *  \param tags : Tags de l'argument recherché.
	 *
	 *  \return La valeur d'un argument avec son tag (à utiliser après parse_arguments).
	 */
	std::string get_argument (const std::vector<std::string> &tags);
	std::string get_arg      (const std::vector<std::string> &tags) { return           get_argument(tags);  }
	int         get_arg_int  (const std::vector<std::string> &tags) { return std::stoi(get_argument(tags)); }
	float       get_arg_float(const std::vector<std::string> &tags) { return std::stof(get_argument(tags)); }

	/*!
	 *  \brief Affiche une aide pour l'utilisation de la commande.
	 */
	void print_usage(                                                );
	void print_usage(std::vector<std::vector<std::string>> arg_groups);

	bool check_arguments();

private:
	/*!
	 *  \brief Retourne vrai si l'argument "m_argv[pos_arg]" est dans args.
	 *
	 *  \param args    : Dictionnaire d'arguments.
	 *  \param pos_arg : La position de l'argument recherché dans m_argv[pos_arg].
	 *
	 *  \return Vrai si l'argument "m_argv[pos_arg]" est dans args.
	 */
	bool sub_parse_arguments(std::map<std::vector<std::string>, std::vector<std::string>> &args,
	                         unsigned short pos_arg);

	bool check_argument(const std::vector<std::string> &tags,
	                          std::map<std::vector<std::string>, std::vector<std::string>> &args);

	/*!
	 *  \brief Clear m_required_args, m_optional_args, m_args and m_doc_args.
	 */
	void clear_arguments();

	void print_usage(const std::vector<std::string> &tags, const std::vector<std::string> &values,
	                 const bool required = false);

	std::vector<std::string> split(std::string str);
};

#endif /* ARGUMENTS_READER_HPP_ */
