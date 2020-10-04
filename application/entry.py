from aiohttp import web
import jinja2
import aiohttp_jinja2

async def create_app():
    """The function defines an application
    and ties it to a compiled source files.
    """

    # create application.
    app = web.Application()

    # set the path to the templates.
    aiohttp_jinja2.setup(
        app,
        loader=jinja2.FileSystemLoader('./application/out/')
    )
    # path to the static files
    app['static_root_url'] = './application/out/'

    # set up the routes
    app.add_routes([
        web.get('/', hello),
        web.static('/', './application/out/', name='static', show_index=True)
    ])

    return app

@aiohttp_jinja2.template('app.html')
async def hello(request):
    return {}

app = create_app()

web.run_app(app)