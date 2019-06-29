void delay(unsigned int times)
{
	unsigned int count;
	int i = 0;
	
	for (; i<times; i++) {
		count = 100000;
		while (count) {
			count--;
		}
	}
}