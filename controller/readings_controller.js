const db = require('../db')

class ReadingsConroller {
    async addReadings(req, res){
        const {date_temp, time_temp, temperature} = req.body;
        const newReadings = await db.query(`INSERT INTO readings (date_temp, time_temp, temperature) values ($1, $2, $3) RETURNING *`, [date_temp, time_temp, temperature]);
        console.log(date_temp, time_temp, temperature);

        res.json(newReadings.rows[0]);
    }

    async getReadings(req, res){
        const readings = await db.query('SELECT * FROM readings');
        res.json(readings.rows);
    }

    async getOneReadings(req, res){
        const id = req.params.id;
        const readings = await db.query('SELECT * FROM readings where id = $1', [id]);
        res.json(readings.rows[0]);
    }

    async updateReadings(req, res){
        const {id, date_temp, time_temp, temperature} = req.body;
        const readings = await db.query(
            'UPDATE readings set date_temp = $1, time_temp = $2, temperature = $3 where id = $4 RETURNING *',
            [date_temp, time_temp, temperature, id]
        );
        res.json(readings.rows[0])
    }

    async deleteReadings(req, res){
        const id = req.params.id;
        const readings = await db.query('DELETE FROM readings where id = $1', [id]);
        res.json(readings.rows[0]);
    }
}

module.exports = new ReadingsConroller();