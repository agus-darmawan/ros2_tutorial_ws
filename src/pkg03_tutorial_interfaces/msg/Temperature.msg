# A custom message representing a temperature reading from a sensor.
# Each line is: <type> <field_name>

string sensor_name        # which sensor produced the reading
float64 temperature       # value in Celsius
float64 humidity          # relative humidity percentage
builtin_interfaces/Time stamp   # when the reading was taken
