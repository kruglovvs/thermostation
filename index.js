const express = require('express');
const readingsRouter = require('./routes/readings_routes')
const PORT = process.env.PORT || 8080;

const app = express();
app.use(express.json()) // Для того, чтобы express смог распарсить json строку
app.use('/api', readingsRouter)

app.listen(PORT, () => console.log(`server started on port ${PORT}`));
