#include <iostream>
#include <message.h>

int main() {
    using std::cout, std::endl, cppchat::api::Message;

    Message test_message;
    test_message.sender = "User1";
    test_message.receiver = "User2";
    test_message.group_id = std::nullopt;
    test_message.content = "Test message";
    test_message.timestamp = time(nullptr);

    cout << "Sender: " << test_message.sender << endl;
    if (test_message.receiver.has_value())
        cout << "Receiver: " << test_message.receiver.value() << endl;
    if (test_message.group_id.has_value())
        cout << "Group ID: " << test_message.group_id.value() << endl;
    cout << "Content: " << test_message.content << endl;
    cout << "Timestamp: " << test_message.timestamp << endl;

    return 0;
}
