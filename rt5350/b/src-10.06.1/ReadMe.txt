/*--------------------------------------------------------------------
������/\~~~~~~~~~~~~~\������������^*^���������$$�� .�� 
����./��\~~~��~�� ~~~~\ ������ʥ�� .���֡� *�� $����$�� * 
����/ ^^ \ �T�T�T�T�T�T\.�������� *��*����*�� $���﨎$�� * 
��..��[]������� �� |������ .����������*�� $��������$�� * 
��&&�������������� ��'|'�� @��������������* $����������$ * 
�� �����������������������{�z�y�xԸ��ʥ�����֦��||��� 


				���� ucp2p ��
�汾��1.0
ʱ�䣺2015.08.01
���ߣ�Faker

--------------------------------------------------------------------*/



1.	��Ҫע����ǣ�ucp2p �������������⣬��ȷ���������Ѿ���װ��
	�ͻ��� �����Ŀ��У� pthread openssl json
			-lpthread -lssl -lcrypto -lm -ljson
	�����������Ŀ��У�	pthread openssl json sqlclient
			-lpthread -lssl -lcrypto -lm -ljson -lmysqlclient
	
	���û�а�װ�ö�Ӧ��lib�⣬��ִ�У�
	apt-get install openssl-dev
	apt-get install libjson0

	��װmysql ���ݿ⡣���尲װ�ο��ٶ�
	1. sudo apt-get install mysql-server
����2. apt-get install mysql-client
����3. sudo apt-get install libmysqlclient-dev

2.	ȷ�����пⶼ��װ�ú�ִ�� make ����

3.	���û������Ļ�������ͨ������ڵ�ǰĿ¼���������漸���ļ�

	p2p_client ��
		P2P�ļ򵥿ͻ���
	p2p_server ��
		P2P�ļ򵥷���������
	
	libp2p-client.so ��
		P2P �ͻ��������Ķ�̬��
	
	libp2p-server.so ��
		P2P �������������Ķ�̬��

4	�� libp2p-client.so libp2p-server.so ������ /usr/lib/ ����

