import React from 'react';
import { Navbar, Nav, NavLink, NavItem } from 'reactstrap';


function PriceHistory(props) {
  return (
    <div className='App-header'>
      <div>
        <Navbar expand='md'>
          <Nav className='Navbar' navbar>            
            <NavItem className='line'>
              <NavLink style={{ color:'white' }} href='/'>Home</NavLink>
            </NavItem>
            <NavItem className='line'>
              <NavLink style={{ color:'white' }} href='/quote'>Quote</NavLink>
            </NavItem>
            <NavItem>
              <NavLink style={{ color:'white' }} href='/price_history'>Price History (restricted)</NavLink>
            </NavItem>
            <NavItem className='navbar-space-right'>
              <NavLink className='line' style={{ color:'white'}} href='/login'>Login</NavLink>
            </NavItem>
            <NavItem>
              <NavLink style={{ color:'white' }} href='/login'>Register</NavLink>
            </NavItem>            
          </Nav>
        </Navbar>
      </div>
      <br></br>
      <br></br>
      <h6 style={{ color:'yellow' }}>
        You need to be logged in to see this content!
      </h6>
    </div>    
  );
}

export default PriceHistory;