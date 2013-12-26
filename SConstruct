
import os

env = Environment()

boost_path = '/tsp/projects/sapc_CBA/design/environment/boost-1.46.1'

env.Append(CPPFLAGS=['-g'])
env.Append(CPPPATH=[os.path.join(boost_path, 'include'), 'common'])
env.Append(LIBPATH=[os.path.join(boost_path, 'lib'), 'lib'])
env.Append(LIBS=['common', 'boost_system', 'boost_program_options'])
env.Append(RPATH=[os.path.join(boost_path, 'lib')])

lib_target = env.Library(target='lib/common', source='common/common.cc')
env.Alias('lib', lib_target)

sync_client_target = env.Program(target='bin/sync_client', source='sync_client/sync_client_main.cc')
env.Alias('sync_client', sync_client_target)

sync_server_target = env.Program(target='bin/sync_server', source='sync_server/sync_server_main.cc')
env.Alias('sync_server', sync_server_target)

async_client_target = env.Program(target='bin/async_client', source=['async_client/async_client_main.cc', 'async_client/TcpClient.cc'])
env.Alias('async_client', async_client_target)

async_server_target = env.Program(target='bin/async_server', source=['async_server/async_server_main.cc', 'async_server/TcpServer.cc'])
env.Alias('async_server', async_server_target)
