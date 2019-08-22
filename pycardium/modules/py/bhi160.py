import sys_bhi160
import interrupt


class BHI160:
    def enable_sensor(self):
        interrupt.disable_callback(self.interrupt_id)
        interrupt.set_callback(self.interrupt_id, self._interrupt)
        self.stream_id = sys_bhi160.enable_sensor(
            self.sensor_id, self.sample_buffer_len, self.sample_rate, self.dynamic_range
        )

        if self.stream_id < 0:
            raise ValueError("Enable sensor returned %i", self.stream_id)

        self.active = True

        if self._callback:
            interrupt.enable_callback(self.interrupt_id)

    def __enter__(self):
        return self

    def __exit__(self, _et, _ev, _t):
        self.close()

    def close(self):
        if self.active:
            self.active = False
            ret = sys_bhi160.disable_sensor(self.sensor_id)

            if ret < 0:
                raise ValueError("Disable sensor returned %i", ret)

            interrupt.disable_callback(self.interrupt_id)
            interrupt.set_callback(self.interrupt_id, None)

    def read(self):
        result = []
        if self.active:
            for sample in sys_bhi160.read_sensor(self.stream_id):
                result.append(self.convert(sample))
        return result

    def _interrupt(self, _):
        if self.active:
            data = self.read()
            print(data)
            if self._callback:
                self._callback(data)


class BHI160Accelerometer(BHI160):
    def __init__(
        self, sample_rate=4, dynamic_range=2, callback=None, sample_buffer_len=200
    ):
        self.sample_rate = sample_rate
        self.dynamic_range = dynamic_range
        self.callback = callback
        self.sample_buffer_len = sample_buffer_len
        self.sensor_id = 0
        self.interrupt_id = interrupt.BHI160_ACCELEROMETER
        self._callback = callback
        self.enable_sensor()

    def convert_single(self, value):
        return 2 * value / 32768.0

    def convert(self, sample):
        return dict(
            {
                "x": self.convert_single(sample[0]),
                "y": self.convert_single(sample[1]),
                "z": self.convert_single(sample[2]),
            }
        )


class BHI160Gyroscope(BHI160):
    def __init__(
        self, sample_rate=4, dynamic_range=2, callback=None, sample_buffer_len=200
    ):
        self.sample_rate = sample_rate
        self.dynamic_range = dynamic_range
        self.callback = callback
        self.sample_buffer_len = sample_buffer_len
        self.sensor_id = 3
        self.interrupt_id = interrupt.BHI160_GYROSCOPE
        self._callback = callback
        self.enable_sensor()

    def convert_single(self, value):
        return 360 * value / 32768.0

    def convert(self, sample):
        return dict(
            {
                "x": self.convert_single(sample[0]),
                "y": self.convert_single(sample[1]),
                "z": self.convert_single(sample[2]),
            }
        )


class BHI160Orientation(BHI160):
    def __init__(
        self, sample_rate=4, dynamic_range=2, callback=None, sample_buffer_len=200
    ):
        self.sample_rate = sample_rate
        self.dynamic_range = dynamic_range
        self.callback = callback
        self.sample_buffer_len = sample_buffer_len
        self.sensor_id = 2
        self.interrupt_id = interrupt.BHI160_ORIENTATION
        self._callback = callback
        self.enable_sensor()

    def convert_single(self, value):
        return 360 * value / 32768.0

    def convert(self, sample):
        return dict(
            {
                "x": self.convert_single(sample[0]),
                "y": self.convert_single(sample[1]),
                "z": self.convert_single(sample[2]),
            }
        )
