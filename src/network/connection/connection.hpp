#include <asio.hpp>

#include "../message/message.hpp"
#include "../queue/queue.hpp"

namespace network
{

template <typename MessageType> class Connection
{
public:
	Connection(){};
	bool connect();
	bool disconnect();
	bool is_connected() const;
	bool send(const Message<MessageType> &message);


private:
	~Connection(){};
	asio::ip::tcp::socket socket;
	asio::io_context &context;
	Queue<Message<MessageType>> input;
	Queue<Message<MessageType>> output;

};

} // namespace network
