#include "JBridge.h"
#include "Util.h"
#include "CommandManager.h"



#include "jni.h"


bool connectJavaAPI() {
	/**
	JavaVM* mJavaVM;
	JNIEnv* env;
	const int size = 3;
	JavaVMOption options[size];
	options[0].optionString = (char*)"-Djava.compiler=NONE";
	options[1].optionString = (char*)"-Djava.class.path=./";

	options[2].optionString = (char*)"-verbose:class";

	JavaVMInitArgs initArgs;
	initArgs.version = JNI_VERSION_1_4;
	initArgs.options = options;
	initArgs.nOptions = size;
	initArgs.ignoreUnrecognized = JNI_TRUE;

	if (JNI_CreateJavaVM(&mJavaVM, (void**)&env, &initArgs) != 0) {
		tout << "unable to create JVM!!" << tendl;
	}
	else {
		tout << "JVM created!" << tendl;
		jclass startClass = env->FindClass("com/hello/HelloWorld");
		jmethodID startMeth;
		if (startClass) {
			tout << "class found!" << tendl;
			startMeth = env->GetStaticMethodID(startClass, "main", "([Ljava/lang/String;)V");
			env->CallStaticVoidMethod(startClass, startMeth, NULL);
		}
		else {
			tout << "class not found!" << tendl;
		}

	}
	/**/
}

void JBridge::process(const string& s) {
	tout << "client " << to_string(s_accept) << ": " << s << tendl;
}

void JBridge::Loop() {
	int recv_len = 0;
	int len = 0;
	char recv_buf[100];


	SOCKADDR_IN accept_addr;

	// set socket to listen
	if (listen(s_server, SOMAXCONN) < 0) {
		tout << "failed to setup socket listener!" << tendl;
		WSACleanup();
		running = false;
	}
	else {
		tout << "made socket to listening mode!" << tendl;
	}
	/**/
	
	// receive data
	while (running) {
		if (!connected) {
			tout << "Ready to connect." << tendl;
			// accept link
			len = sizeof(SOCKADDR);
			s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
			if (s_accept == SOCKET_ERROR) {
				tout << "failed to connect!" << tendl;
			}
			else {
				tout << "Link established, ready to accept data." << tendl;
				connected = true;
			}

			Send("Connection established.");
		}

		recv_len = recv(s_accept, recv_buf, 100, 0);
		if (recv_len < 0) {
			tout << "Connection broke." << tendl;
			connected = false;
		}
		else {
			string message(recv_buf);
			message.erase(message.begin(), std::find_if(message.begin(), message.end(), [](unsigned char ch) {
				return !std::isspace(ch);
			}));
			message.erase(std::find_if(message.rbegin(), message.rend(), [](unsigned char ch) {
				return !std::isspace(ch);
			}).base(), message.end());

			process(message);
		}
		Sleep(3);
	}
}







bool JBridge::running;
bool JBridge::connected;
std::thread JBridge::socketThread;
SOCKET JBridge::s_server;
SOCKET JBridge::s_accept;

bool JBridge::Send(string s) {
	const char* send_buf = s.c_str();
	int send_len = send(s_accept, send_buf, s.length(), 0);

	if (send_len < 0) {
		tout << "Failed to send! Message: " << s << tendl;
		return false;
	}

//	tout << "Message sent to " << to_string(s_accept) << tendl;

//	tout << to_string(s.length()) << ", " << to_string(send_len) << tendl;
	return true;
}

bool JBridge::Init() {
	// server and user address
	SOCKADDR_IN server_addr;


	WORD w_req = MAKEWORD(2, 2);// version
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		tout << "failed to create socket lib" << tendl;
		return false;
	}
	else {
		tout << "socket lib created" << tendl;
	}
	// check socket version
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		tout << "socket version not match" << tendl;
		WSACleanup();
		return false;
	}
	else {
		tout << "socket version correct" << tendl;
	}

	// fill server info
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(5010);

	// create socket
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		tout << "failed to create socket!" << tendl;
		WSACleanup();
		return false;
	}

	running = true;
	connected = false;
	socketThread = thread(Loop);


	CommandManager::registerCmd("send", [](Command cmd)->void {
		Send(to_str(cmd.argFullText));
	});

	return true;
}

void JBridge::Dispose() {
	running = false;

	// close socket
	closesocket(s_server);
	closesocket(s_accept);

	// release dll resources
	WSACleanup();
	socketThread.join();

}