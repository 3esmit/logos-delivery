import importlib.util
import sys
import types
import unittest
from pathlib import Path


class Metric:
    def __init__(self, *args, **kwargs):
        pass

    def labels(self, **kwargs):
        return self

    def inc(self):
        pass

    def set(self, value):
        pass


def install_optional_dependency_stubs():
    if importlib.util.find_spec('requests') is None:
        requests = types.ModuleType('requests')

        class RequestException(Exception):
            pass

        requests.RequestException = RequestException
        sys.modules['requests'] = requests

    if importlib.util.find_spec('prometheus_client') is None:
        prometheus_client = types.ModuleType('prometheus_client')
        prometheus_client.Counter = Metric
        prometheus_client.Gauge = Metric
        prometheus_client.start_http_server = lambda *args, **kwargs: None
        sys.modules['prometheus_client'] = prometheus_client


def load_sonda():
    install_optional_dependency_stubs()
    module_path = Path(__file__).with_name('sonda.py')
    spec = importlib.util.spec_from_file_location('sonda_under_test', module_path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


class SondaStoreQueryTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.sonda = load_sonda()

    def test_rest_store_timeout_is_a_peer_query_timeout(self):
        failures = []
        self.sonda.record_store_failure = lambda *args, **kwargs: failures.append(
            (args, kwargs)
        )
        self.sonda.requests.get = lambda *args, **kwargs: types.SimpleNamespace(
            status_code=500,
            text='No history response received (timeout)',
        )

        result = self.sonda.send_store_query(
            'http://node', 'peer-a', 'pubsub', 'content', 1
        )

        self.assertFalse(result)
        self.assertEqual(
            failures,
            [
                (
                    (
                        'peer-a',
                        self.sonda.ERR_QUERY_TIMEOUT,
                        'No history response received (timeout)',
                    ),
                    {},
                )
            ],
        )

    def test_unrelated_rest_500_remains_local_unknown(self):
        failures = []
        self.sonda.record_store_failure = lambda *args, **kwargs: failures.append(
            (args, kwargs)
        )
        self.sonda.requests.get = lambda *args, **kwargs: types.SimpleNamespace(
            status_code=500,
            text='unrelated internal error',
        )

        result = self.sonda.send_store_query(
            'http://node', 'peer-a', 'pubsub', 'content', 1
        )

        self.assertFalse(result)
        self.assertEqual(
            failures,
            [
                (
                    (
                        'peer-a',
                        self.sonda.ERR_UNKNOWN,
                        'HTTP 500 unrelated internal error',
                    ),
                    {'blame_peer': False},
                )
            ],
        )


if __name__ == '__main__':
    unittest.main()
