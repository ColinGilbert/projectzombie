from multiprocessing import Process

class fc():

    def __init__(self):
        return

    def run(self):
        print "hello run"
        return

def f(name):
    print "hello", name

if __name__ == '__main__':
    f = fc()
    p = Process(target=f.run)
    p.start()
    p.join()
