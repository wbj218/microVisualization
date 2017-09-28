import sys
sys.path.append('../gen-py/')

from NetflixMicroservices import ProcessMovieID
from NetflixMicroservices import ProcessUniqueID
from NetflixMicroservices import ProcessText
from NetflixMicroservices import AssignRating


from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

def main():

    text_socket = TSocket.TSocket('localhost', 9092)
    text_transport = TTransport.TBufferedTransport(text_socket)
    text_protocol = TBinaryProtocol.TBinaryProtocol(text_transport)
    text_client = ProcessText.Client(text_protocol)


    rating_socket = TSocket.TSocket('localhost', 9093)
    rating_transport = TTransport.TBufferedTransport(rating_socket)
    rating_protocol = TBinaryProtocol.TBinaryProtocol(rating_transport)
    rating_client = AssignRating.Client(rating_protocol)


    unique_id_socket = TSocket.TSocket('localhost', 9091)
    unique_id_transport = TTransport.TBufferedTransport(unique_id_socket)
    unique_id_protocol = TBinaryProtocol.TBinaryProtocol(unique_id_transport)
    unique_id_client = ProcessUniqueID.Client(unique_id_protocol)


    movie_id_socket = TSocket.TSocket('localhost', 9094)
    movie_id_transport = TTransport.TBufferedTransport(movie_id_socket)
    movie_id_protocol = TBinaryProtocol.TBinaryProtocol(movie_id_transport)
    movie_id_client = ProcessMovieID.Client(movie_id_protocol)


    for i in range (5):
        for j in range(5):
            text_transport.open()
            text_client.process_text("req_" + str(i), "user_" + str(i), "This is a review.")
            text_transport.close()

            unique_id_transport.open()
            unique_id_client.get_unique_id("req_" + str(i), "user_" + str(i) + str(i))
            unique_id_transport.close()

            movie_id_transport.open()
            movie_id_client.process_movie_id("req_" + str(i), "user_" + str(i), "http://www.imdb.com/title/movie_" + str(j))
            movie_id_transport.close()

            rating_transport.open()
            rating_client.assign_rating("req_" + str(i), "user_" + str(i), "5")
            rating_transport.close()






if __name__ == '__main__':
    try:
        main()
    except Thrift.TException as tx:
        print('%s' % tx.message)





