/* 
 *  odeJSON: ODE parser plugin for PopJSON.
 * 
 *  Copyright (C) 2026  Kamil Erguler <kerguler@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

'use strict';

const { PopJSON } = require('./index.js');

class PetriJSON extends PopJSON {

    parse_old() {
        this.json = super.handle_repeats(this.json);
        //
        const that = this;
        super.ids = [];
        //
        if (!('environ' in this.json)) this.json['environ'] = [];
        this.environs = this.json['environ'].map( (pr) => super.check_ids(pr['id']) );
        this.numenv = this.environs.length;
        //
        this.parametersv = this.json['parameters'].filter( (p) => !p['constant'] ).map( (pr) => super.check_ids(pr['id']) );
        this.parametersc = this.json['parameters'].filter( (p) => p['constant'] ).map( (pr) => pr['id'] );
        if (!('functions' in this.json)) this.json['functions'] = [];
        this.numpar = this.json['parameters'].filter( (p) => !p['constant'] ).length;
        //
        this.functions = Object.keys(this.json['functions']);
        //
        this.numint = 0;
        //
        if (!('populations' in this.json)) {
            this.json['populations'] = [];
            this.error += "Couldn't find any populations!\n";
            return(super.results());
        }
        this.populations = this.json['populations'].map( (pr) => super.check_ids(pr['id']) );
        this.numpop = this.json['populations'].length;
        //
        this.processobj = {};
        this.intermediates = [];
        this.transformations = [];
        this.transfers = [];
        //
        this.deterministic = true;
        //
        this.operations = ["abs","min","max","round","poisson","binomial","define","?","&&","||",">=","<=",">","<","==","sqrt","pow","exp","log","log2","log10","indicator","index","size","count","*","+","-","/","%","!"];
        this.funparnames = ["a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
                            "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]
        //
        this.header = "";
        this.model = "";
        if (!this.error)
            this.write_ode_petrinet();
    }

}

module.exports = { PetriJSON };