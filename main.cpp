int main (int argc, char *argv[])
{
	if (argc != 3)
		return 1;
	try
	{
		Irc(argv[0], argv[1]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}