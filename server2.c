//Servidor [AREYCI HUERTA PATIÑO]

#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include <gtk/gtk.h>
#include <pthread.h>

int clientSocketDescriptor,socketDescriptor;

struct sockaddr_in serverAddress,clientAddress;
char recvBuffer[1000],sendBuffer[1000];
pid_t cpid;
char aprobacion[1024];

GtkWidget *entry;
GtkWidget *income;

//Gtk Widgets
GtkWidget *message, *username; //objetos que reciben información
GtkWidget *label_message; //objetos que arrojan resultado 

void error(char *msg){
	perror(msg);
	exit(0);
}




void quit(GtkWidget *widget, gpointer *data)
{
	send(socketDescriptor, "e", 1, 0);
	gtk_main_quit();
}

void *myThreadFun(void *vargp) 
{ 
socklen_t clientLength;


bzero(&serverAddress,sizeof(serverAddress));
/*Estructura de dirección de socket*/
serverAddress.sin_family=AF_INET;
serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
serverAddress.sin_port=htons(5000);
/*El socket TCP es creado, una estructura de dirección de Internet de socker es llenada
	con una dirección como comodín y el puerto ya conocido del servidor*/
socketDescriptor=socket(AF_INET,SOCK_STREAM,0);
/*Función bind asigna una dirección de protocolo local al socket*/
bind(socketDescriptor,(struct sockaddr*)&serverAddress,sizeof(serverAddress));
/*La función listen específica el número máximos de conexiones que el kernel debe
	enviar a la cola para este socket*/
listen(socketDescriptor,5);
printf("%s\n","Servidor en ejecucion ...");
/*El servidor regresa la siguiente conexión completada desde el frente de la conexión completada,
	Queue lo llama*/
clientSocketDescriptor=accept(socketDescriptor,(struct sockaddr*)&clientAddress,&clientLength);
cpid = fork();
printf("/////////////%d\n",cpid);
} 

int main(int argc,char *argv[])
{
//Gtk Widgets
GtkWidget *ventana, *descripcion[15], *layout, *send_button, *file_button;  /*descripción = labels, layout=apariencia*/
GtkTextBuffer *buffer;
GtkWidget *vbox;	
gtk_init(&argc, &argv);  //inicializa GTK
int i; 
pthread_t tid; 
pthread_create(&tid, NULL, myThreadFun, (void *)&tid);



void *receive_message(void *data)
{
	char aux[1024];
	sprintf(aprobacion, "%s",strcat(aprobacion,"\nAmigo: "));
	sprintf(aux, "%s",strcat(aprobacion,"\n"));
	
	//gtk_text_buffer_set_text (buffer, strcat(aprobacion, gtk_entry_get_text(GTK_ENTRY(message))), -1);
	//bzero(&serverAddress,sizeof(serverAddress));
	bzero(&recvBuffer,sizeof(recvBuffer));
	// *data is of no use
	int n;

	n = recv(clientSocketDescriptor, recvBuffer, sizeof(recvBuffer), 0);
	//recv(clientSocketDescriptor,recvBuffer,sizeof(recvBuffer),0);
	printf("\nCliente: %s\n",recvBuffer);
	if (n < 1) error("Leyendo desde cliente");
	recvBuffer[n]='\0';
	//gtk_entry_set_text(GTK_ENTRY(income), recvBuffer);
	gtk_text_buffer_set_text (buffer, strcat(aprobacion,recvBuffer), -1);
	return NULL;
}

		
void send_message(){
		char aux[1024];
		sprintf(aprobacion, "%s",strcat(aprobacion,gtk_entry_get_text(GTK_ENTRY(username))));
		sprintf(aux, "%s",strcat(aprobacion,":"));
		sprintf(aux, "%s",strcat(aprobacion,"\n"));
		gtk_text_buffer_set_text (buffer, strcat(aprobacion,gtk_entry_get_text(GTK_ENTRY(message))), -1);
		//gtk_text_buffer_set_text (buffer, strcat(aprobacion, gtk_entry_get_text(GTK_ENTRY(message))), -1);
		//bzero(&serverAddress,sizeof(serverAddress));
		bzero(&sendBuffer,sizeof(sendBuffer));
		//printf("\nType a message here ...  ");
		/*Read the message from client*/
		//fgets(sendBuffer,10000,stdin);
	
		int n;
		const gchar *msg = gtk_entry_get_text(GTK_ENTRY(message));
		n = send(clientSocketDescriptor, msg, strlen(msg)+1,0);
		/*Sends the message to client*/
		//send(clientSocketDescriptor,sendBuffer,strlen(sendBuffer)+1,0);
		printf("\nMensaje Enviado!\n");
		if (n < strlen(msg))
			error("Escribiendo al cliente");
}
	
	//Creación de nuestra ventana
	ventana = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(ventana, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_window_set_title(GTK_WINDOW(ventana),"Servidor");
	gtk_container_set_border_width(GTK_CONTAINER(ventana),0);
	gtk_window_set_default_size(GTK_WINDOW(ventana),550,500);
	
	//Creación de la apariencia(layout)
	layout = gtk_layout_new(NULL, NULL);	
	
	//Creación de las descripciones dentro de la ventana
	descripcion[0] = gtk_label_new("SERVIDOR");
	descripcion[1] = gtk_label_new("@AHP");
	descripcion[2] = gtk_label_new("Nombre de usuario: ");
	descripcion[3] = gtk_label_new("Mensaje: ");
	
	label_message = gtk_text_view_new ();
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (label_message));
	vbox = gtk_box_new(TRUE,0);

	
	//Creación de campos de texto
	message = gtk_entry_new();
	username = gtk_entry_new();
	
	//Formato campos de texto
	gtk_entry_set_max_length(GTK_ENTRY(message),30);
	gtk_entry_set_max_length(GTK_ENTRY(username), 30);
	
	//Tamaño del campo de texto 
	gtk_entry_set_width_chars(GTK_ENTRY(message),30);
	gtk_entry_set_width_chars(GTK_ENTRY(username), 30);
	
	
	//Creación de los botones
	send_button = gtk_button_new_with_label("Enviar");
	g_signal_connect(send_button, "clicked", G_CALLBACK(send_message),NULL);
	g_signal_connect(send_button, "clicked", G_CALLBACK(receive_message),NULL);
	//file_button = gtk_button_new_with_label("Archivo");
	//g_signal_connect(file_button, "clicked", G_CALLBACK(send_file),NULL);
	
	//Ensamblamos todos nuestros objetos que hemos creado
	gtk_layout_put(GTK_LAYOUT(layout),descripcion[0], 250, 10 );
	gtk_layout_put(GTK_LAYOUT(layout),descripcion[1], 250, 50 );
	gtk_layout_put(GTK_LAYOUT(layout),descripcion[2], 10, 90 );
	gtk_layout_put(GTK_LAYOUT(layout),descripcion[3], 10, 130 );
	
	//Colocar los demás objetos
	gtk_layout_put(GTK_LAYOUT(layout), message , 200 , 130 );
	gtk_layout_put(GTK_LAYOUT(layout), username , 200, 90  );
	gtk_layout_put(GTK_LAYOUT(layout), label_message , 200 , 180 );
	gtk_layout_put(GTK_LAYOUT(layout), vbox, 200,180);
	
	//botones 
	gtk_layout_put(GTK_LAYOUT(layout),send_button, 460, 130);
	//gtk_layout_put(GTK_LAYOUT(layout),file_button, 460, 170);
	
	
	//se muestra la ventana 
	gtk_container_add(GTK_CONTAINER(ventana),layout);
	
	gtk_widget_show_all(ventana);
	
	gtk_main();

	
return 0;
}
