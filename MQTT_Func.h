#define ADDRESS     "tcp://192.168.0.113:1883"
#define CLIENTID    "MiddleNode"
#define QOS         1
#define TIMEOUT     10000L

static volatile bool isMessageArrived = false;

/* Buffer size for a receiving message. */
const int MESSAGE_BUFFER_SIZE = 512;
/* Buffer for a receiving message. */
char messageBuffer[MESSAGE_BUFFER_SIZE];

//char buf[MESSAGE_BUFFER_SIZE];

char TOPIC[40] = "TEST";
volatile MQTTClient_deliveryToken deliveredtoken;

void create_Topic(char* tpc_ty, char* orignator, char* receiver);
void delivered(void *context, MQTTClient_deliveryToken dt);
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);
void connlost(void *context, char *cause);
void publish(MQTTClient client, const char* buffer);

void create_Topic(char* tpc_ty, char* orignator, char* receiver)
{
    sprintf(TOPIC,"/oneM2M/%s/%s/%s",tpc_ty,orignator,receiver);
    printf("\nTopic is %s\n", TOPIC);
}

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    isMessageArrived = true;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: %s",message->payload);

    strcpy(messageBuffer, (const char*)message->payload);
    /*for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }*/
    putchar('\n');
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}
void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

void publish(MQTTClient client, const char* buffer)
{	
	int rc;
	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;
	pubmsg.payload = (void*) buffer;
    pubmsg.payloadlen = (int)strlen(buffer);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    printf("\nWaiting for up to %d seconds for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            (int)(TIMEOUT/1000), buffer, TOPIC, CLIENTID);
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);
	return;
}
