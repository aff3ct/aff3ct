#ifndef TERMINAL_HPP_
#define TERMINAL_HPP_

class Terminal
{
public:
	Terminal(const std::string name = "Terminal") {}
	virtual ~Terminal() {}

	virtual void legend      (std::ostream &stream) { };
	virtual void temp_report (std::ostream &stream) = 0;
	virtual void final_report(std::ostream &stream) = 0;
};

#endif /* TERMINAL_HPP_ */
