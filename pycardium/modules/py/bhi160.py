import sys_bhi160
import interrupt


class BHI160Accelerometer:
    def __init__(
        self, sample_rate=4, dynamic_range=2, callback=None, sample_buffer_len=200
    ):
        interrupt.disable_callback(interrupt.BHI160_ACCELEROMETER)
        interrupt.set_callback(
            interrupt.BHI160_ACCELEROMETER, self._accelerometer_interrupt
        )
        self.acc_sd = sys_bhi160.enable_sensor(
            0, sample_buffer_len, sample_rate, dynamic_range
        )
        self._callback = callback
        if callback:
            interrupt.enable_callback(interrupt.BHI160_ACCELEROMETER)

    def __enter__(self):
        return self

    def __exit__(self, _et, _ev, _t):
        self.close()

    def close(self):
        if self.acc_sd is not None:
            self.acc_sd = None
            self.acc_sd = sys_bhi160.disable_sensor(0)
            interrupt.disable_callback(interrupt.BHI160_ACCELEROMETER)
            interrupt.set_callback(interrupt.BHI160_ACCELEROMETER, None)

    def _accelerometer_interrupt(self, _):
        if self.acc_sd is not None:
            data = sys_bhi160.read_sensor(self.acc_sd)
            print(data)
            if self._callback:
                self._callback(data)
