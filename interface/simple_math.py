from ctypes import cdll
smath = cdll.LoadLibrary("./libSimpleMath.so")

class SimpleMath:

    def __init__(self):
        self.cObj = smath.smath_new()

# END Class SimpleMath
