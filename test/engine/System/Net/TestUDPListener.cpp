
#include "System/Net/UDPListener.h"

#define BOOST_TEST_MODULE UDPListener
#include <boost/test/unit_test.hpp>

static inline bool TryBindAddr(netcode::SocketPtr& socket, const char* address) {
	return netcode::UDPListener::TryBindSocket(11111, &socket, address);
}

static inline bool TryBindPort(netcode::SocketPtr& socket, int port) {
	return netcode::UDPListener::TryBindSocket(port, &socket, "::");
}

BOOST_AUTO_TEST_CASE(TryBindSocket)
{
	netcode::SocketPtr socket;

	// IP v4 & v6 addresses
	BOOST_CHECK(TryBindAddr(socket, "127.0.0.1"));
	BOOST_CHECK(TryBindAddr(socket, "0.0.0.0"));
	BOOST_CHECK(TryBindAddr(socket, "::"));
	BOOST_CHECK(TryBindAddr(socket, "::1"));

	// badly named invalid IP v6 addresses
	BOOST_CHECK(!TryBindAddr(socket, "::2"));
	BOOST_CHECK(!TryBindAddr(socket, "fe80::224:1dff:fecf:df44/64"));
	BOOST_CHECK(!TryBindAddr(socket, "fe80::224:1dff:fecf:df44"));
	BOOST_CHECK(!TryBindAddr(socket, "::224:1dff:fecf:df44/64"));
	BOOST_CHECK(!TryBindAddr(socket, "fe80::"));
	BOOST_CHECK(!TryBindAddr(socket, "224:1dff:fecf:df44/64"));
	BOOST_CHECK(!TryBindAddr(socket, "fe80"));

	// host-names (only addresses are allowed)
	BOOST_CHECK(!TryBindAddr(socket, "localhost"));
	BOOST_CHECK(!TryBindAddr(socket, "local.lan"));
	BOOST_CHECK(!TryBindAddr(socket, "google.com"));

	// normal ports
	BOOST_CHECK(TryBindPort(socket, 1024));
	BOOST_CHECK(TryBindPort(socket, 11111));
	BOOST_CHECK(TryBindPort(socket, 32000));
	BOOST_CHECK(TryBindPort(socket, 65535));

	// special ports (reserved for core services)
	BOOST_WARN(!TryBindPort(socket, 0));
	BOOST_WARN(!TryBindPort(socket, 1));
	BOOST_WARN(!TryBindPort(socket, 128));
	BOOST_WARN(!TryBindPort(socket, 1023));

	// out-of-range ports
	BOOST_CHECK(!TryBindPort(socket, 65536));
	BOOST_CHECK(!TryBindPort(socket, 65537));
	BOOST_CHECK(!TryBindPort(socket, -1));
}
