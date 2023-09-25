#include <iostream>
#include <string>
#include <librdkafka/rdkafkacpp.h>
#include <rapidjson/document.h>

class KafkaConsumer {
public:
    KafkaConsumer(const std::string& brokers, const std::string& topic) : brokers_(brokers), topic_(topic) {}

    void Consume() {
        RdKafka::Consumer *consumer = nullptr;
        RdKafka::Topic *topic = nullptr;
        RdKafka::Message *message = nullptr;

        RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
        conf->set("metadata.broker.list", brokers_, errstr_);

        consumer = RdKafka::Consumer::create(conf, errstr_);
        if (!consumer) {
            std::cerr << "Failed to create Kafka consumer: " << errstr_ << std::endl;
            exit(1);
        }

        topic = RdKafka::Topic::create(consumer, topic_, nullptr, errstr_);
        if (!topic) {
            std::cerr << "Failed to create Kafka topic: " << errstr_ << std::endl;
            exit(1);
        }

        consumer->start(topic, 0, RdKafka::Topic::OFFSET_END);

        while (true) {
            message = consumer->consume(topic, 0, 1000);
            if (message->err()) {
                if (message->err() != RdKafka::ERR__PARTITION_EOF) {
                    std::cerr << "Consume error: " << message->errstr() << std::endl;
                }
                delete message;
                continue;
            }

            // Parse JSON message using RapidJSON
            rapidjson::Document doc;
            doc.Parse(static_cast<const char *>(message->payload()));

            if (doc.HasParseError()) {
                std::cerr << "JSON parse error: " << GetParseError_En(doc.GetParseError()) << std::endl;
            } else {
                // Access JSON data as needed
                if (doc.HasMember("key")) {
                    const rapidjson::Value& key = doc["key"];
                    if (key.IsString()) {
                        std::cout << "Received message key: " << key.GetString() << std::endl;
                    }
                }

                if (doc.HasMember("value")) {
                    const rapidjson::Value& value = doc["value"];
                    if (value.IsString()) {
                        std::cout << "Received message value: " << value.GetString() << std::endl;
                    }
                }
            }

            delete message;
        }

        delete topic;
        delete consumer;
    }

private:
    std::string brokers_;
    std::string topic_;
    std::string errstr_;
};

int main() {
    std::string brokers = "your_kafka_brokers";
    std::string topic = "your_kafka_topic";

    KafkaConsumer consumer(brokers, topic);
    consumer.Consume();

    return 0;
}
